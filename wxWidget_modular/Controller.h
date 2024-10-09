#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>

class Controller {
public:
    static bool Login(const std::string& username, const std::string& password);
    static bool Register(const std::string& username, const std::string& password);
};

#endif // CONTROLLER_H
