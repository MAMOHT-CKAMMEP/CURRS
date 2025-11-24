#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <unordered_map>
#include <vector>

class Server {
public:
    Server(int port, const std::string& userDbPath, const std::string& logPath);
    bool start();

    
private:
    int port;
    std::string userDbPath;
    std::string logPath;
    std::unordered_map<std::string, std::string> users;
    
    void loadUserDatabase();
    void logError(const std::string& message, bool isCritical);
    void handleClient(int clientSocket);
    bool authenticate(int clientSocket);
    void processVectors(int clientSocket);
    int16_t calculateProduct(const std::vector<int16_t>& vector);
    std::string md5Hash(const std::string& input);
};

#endif