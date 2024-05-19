
#ifndef SSH_MANAGER_H // include guard
#define SSH_MANAGER_H

#include <libssh/libssh.h>
#include <string>
#include <atomic>
#include <unordered_map>
#include <chrono>
#include <iostream>
#include <mutex>
#include <condition_variable>



class Connection {

    enum Status {
        UNKNOWN = 0,
        DISCONNECTED = 1,
        IDLE = 2,
        BUSY = 3,
    };


private:
    ssh_session _session;
    std::string _hostname, _username, _password, _handle;
    bool _isConnected;
    //bool _busy;
    bool _cancelConnectionAttempt;

    

    bool _waitForConnection();
    void _attemptConnection();

    // Lifetime tracking variables
    std::chrono::steady_clock::time_point _connectionStartTime;
    std::chrono::steady_clock::time_point _disconnectionTime;

    Status _status;

public:
    
    const std::string& getHandle() const { return _handle; }


    std::string getStatusStr();
    std::chrono::duration<double> getConnectionLifetime();
    Status getStatus() { return _status; }

    Connection(int& instance_count, const std::string& hostname, const std::string& username, const std::string& password);

    
    void Connect();
    void Disconnect();

    bool isConnected() { return _isConnected; };
    bool isBusy() { return _status == Status::BUSY || Status::UNKNOWN; }

    std::string getConnectionLifeTimeStr();

    std::string executeCommand(const std::string& command);



};

class SSHManager
{
private:
    int _instance_count; 

    std::vector<Connection> _connections;

public:

	SSHManager();

    ~SSHManager();


    void addConnection(const std::string& hostname, const std::string& username, const std::string& password);

   

    void removeConnection(std::string& handle);
    void removeConnection(const Connection& connection);


    Connection* getConnection(std::string& handle);

    Connection* getFree();

    std::vector<Connection*> listConnections();


    

    
};

#endif