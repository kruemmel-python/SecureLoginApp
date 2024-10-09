#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

class Logger {
public:
    static void Log(const std::string& message) {
        std::ofstream logFile("log.txt", std::ios_base::app);
        if (logFile.is_open()) {
            logFile << message << std::endl;
        }
    }
};

#endif // LOGGER_H
