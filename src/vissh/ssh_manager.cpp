#include "ssh_manager.h"

#include <ostream>
#include <utilities/identification.h>


std::string Connection::getStatusStr()
{
    switch (_status)
    {
    case Connection::UNKNOWN:
        return "unknown";
        break;
    case Connection::DISCONNECTED:
        return "disconnected";
        break;
    case Connection::IDLE:
        return "idle";
        break;
    case Connection::BUSY:
        return "busy";
        break;


    default:
        return "unknown";
        break;
    }
}


std::chrono::duration<double> Connection::getConnectionLifetime() {
    if (_isConnected) {
        return std::chrono::steady_clock::now() - _connectionStartTime;
    }
    else {
        return _disconnectionTime - _connectionStartTime;
    }
}



SSHManager::SSHManager()
{

}

SSHManager::~SSHManager()
{
    for (auto& connection : _connections) {

        connection.Disconnect();
    }
}

void SSHManager::addConnection(const std::string& hostname, const std::string& username, const std::string& password)
{
    Connection connection = Connection(_instance_count, hostname, username, password);

    _connections.push_back(connection);
}


void SSHManager::removeConnection(std::string& handle)
{
    // Use std::remove_if and lambda to remove the matching connection
    auto newEnd = std::remove_if(_connections.begin(), _connections.end(),
        [&handle](const Connection& conn) {
            return conn.getHandle() == handle;
        });

    // Actually erase the removed elements from the container
    _connections.erase(newEnd, _connections.end());
}

void SSHManager::removeConnection(const Connection& connection) {
    auto newEnd = std::remove_if(_connections.begin(), _connections.end(),
        [&connection](const Connection& currentConnection) {
            return &currentConnection == &connection;
        }
    );
    _connections.erase(newEnd, _connections.end());
}

Connection* SSHManager::getConnection(std::string& handle)
{
    for (auto& connection : _connections) {
        if (connection.getHandle() == handle) {
            return &connection;
        }
    }
}

//Get first non-busy connection
Connection* SSHManager::getFree()
{
    for (auto& connection : _connections) {
        if (not connection.isBusy()) {
            return &connection;
        }
    }
}


std::vector<Connection*> SSHManager::listConnections()
{
    std::vector<Connection*> strList;
    for (auto& connection : _connections) {
        strList.push_back(&connection);
    }

    return strList;
}





Connection::Connection(int& instance_count, const std::string& hostname, const std::string& username, const std::string& password)
{
    this->_hostname = hostname;
    this->_username = username;
    this->_password = password;
    _cancelConnectionAttempt = false;
    _isConnected             = false;
    _session                 = nullptr;
    _status                  = Status::UNKNOWN;

    _handle = generateHandle("CH");

}
std::mutex _mtx;
std::condition_variable _cv;

void Connection::_attemptConnection() {
    {
        std::lock_guard<std::mutex> lock(_mtx);
        if (_isConnected || _cancelConnectionAttempt) {
            std::cout << "SSHManager | Connection " << _handle << " already connected or cancellation requested" << std::endl;
            return; // Already connected or cancellation requested
        }
    }

    _session = ssh_new();
    if (!_session) {
        std::cerr << "Failed to initialize SSH session." << std::endl;
        std::cout << "SSHManager | Connection " << _handle << " failed to initialize SSH session." << std::endl;

        return;
    }

    ssh_options_set(_session, SSH_OPTIONS_HOST, _hostname.c_str());
    ssh_options_set(_session, SSH_OPTIONS_USER, _username.c_str());

    if (ssh_connect(_session) != SSH_OK) {
        ssh_free(_session);
        _session = nullptr;
        std::cerr << "SSH connection failed." << std::endl;
        std::cout << "SSHManager | Connection " << _handle << " SSH connection failed." << std::endl;

        return;
    }

    if (ssh_userauth_password(_session, nullptr, _password.c_str()) != SSH_AUTH_SUCCESS) {
        ssh_disconnect(_session);
        ssh_free(_session);
        _session = nullptr;
        std::cerr << "SSH authentication failed." << std::endl;
        std::cout << "SSHManager | Connection " << _handle << " SSH authentication failed." << std::endl;
        return;
    }

    {
        std::lock_guard<std::mutex> lock(_mtx);
        _isConnected = true;
        _cv.notify_all();
        _status = Connection::Status::IDLE;
        std::cout << "SSHManager | Connection " << _handle << " SSH connection successful." << std::endl;

        _connectionStartTime = std::chrono::steady_clock::now();
    }
}

void Connection::Connect() {
    std::cout << "SSHManager | Connection " << _handle << " SSH connecting..." << std::endl;
    std::thread connectionThread(&Connection::_attemptConnection, this);
    connectionThread.detach(); // Consider alternatives to detaching, if possible
}


void Connection::Disconnect()
{

    if (_session != nullptr) {
        ssh_disconnect(_session);
        ssh_free(_session);
        _session = nullptr;
    }
    _isConnected = false;
    _status = Connection::Status::DISCONNECTED;
    _disconnectionTime = std::chrono::steady_clock::now();

}


bool Connection::_waitForConnection()
{
    std::unique_lock<std::mutex> lock(_mtx);
    return _cv.wait_for(lock, std::chrono::milliseconds(1000), [this] { return _isConnected; });
}

std::string Connection::getConnectionLifeTimeStr()
{
    auto lifetime = getConnectionLifetime();
    int hours = std::chrono::duration_cast<std::chrono::hours>(lifetime).count();
    int minutes = std::chrono::duration_cast<std::chrono::minutes>(lifetime).count() % 60;
    int seconds = std::chrono::duration_cast<std::chrono::seconds>(lifetime).count() % 60;

    char lifetimeText[50];
    snprintf(lifetimeText, sizeof(lifetimeText), "%02d:%02d:%02d", hours, minutes, seconds);

    return std::string(lifetimeText);
}

std::string Connection::executeCommand(const std::string& command)
{
    if (!_waitForConnection()) {

        std::cout << "SSHManager | Connection" << _handle <<" is not active";
        return "ERROR";
    }

    // Outputs the result as string
    ssh_channel channel;
    char buffer[256];
    int nbytes;
    int rc;
    std::string output;

    channel = ssh_channel_new(_session);
    if (channel == nullptr) {
        throw std::runtime_error("Failed to create new channel");
    }

    rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        throw std::runtime_error(ssh_get_error(_session));
    }

    rc = ssh_channel_request_exec(channel, command.c_str());
    if (rc != SSH_OK) {
        ssh_channel_free(channel);
        throw std::runtime_error(ssh_get_error(_session));
    }

    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    while (nbytes > 0) {
        output.append(buffer, nbytes);
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0);
    }

    if (nbytes < 0) {
        ssh_channel_free(channel);
        throw std::runtime_error("Error reading from channel");
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return output;
}
