#pragma once

#include <string>

struct EDISESettings {
    std::string sshHostname;
    std::string sshUsername;
    std::string sshPassword;
    double sshFrequency = 1.0;
};

struct SSHProfile {
    std::string profileID;
    std::string sshHostname;
    std::string sshUsername;
    std::string sshPassword;
};