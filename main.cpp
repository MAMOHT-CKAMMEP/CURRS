/**
 * @file main.cpp
 * @author Полежаев А.И.
 * @date 15.12.2025
 * @brief Точка входа серверного приложения.
 * @details Основная программа, которая парсит аргументы командной строки,
 *          создает и запускает экземпляр сервера для обработки клиентских подключений.
 */

#include <iostream>
#include <cstring>
#include "server.h"

/**
 * @brief Выводит справочную информацию о параметрах командной строки.
 * @details Отображает список доступных опций для запуска сервера и их значения по умолчанию.
 */
void showHelp() {
    std::cout << "Usage: server [OPTIONS]\n"
              << "Options:\n"
              << "  -h              Show this help\n"
              << "  -p PORT         Port number (default: 33333)\n"
              << "  -c CONFIG_FILE  User database file (default: ./vcalc.conf)\n"
              << "  -l LOG_FILE     Log file (default: ./log/vcalc.log)\n";
}

/**
 * @brief Основная функция серверного приложения.
 * @param argc Количество аргументов командной строки.
 * @param argv Массив строк-аргументов.
 * @return Код завершения программы:
 *         - 0: успешное завершение или показ справки
 *         - 1: ошибка запуска сервера
 */
int main(int argc, char* argv[]) {
    int port = 33333;
    std::string configFile = "./vcalc.conf";
    std::string logFile = "./log/vcalc.log";
    
    // Если нет аргументов или есть -h, показываем справку и выходим
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) {
            showHelp();
            return 0;  // Выходим сразу!
        }
    }
    
    if (argc == 1) {
        showHelp();
        return 0;  // Выходим сразу!
    }
    
    // Парсим остальные аргументы
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            configFile = argv[++i];
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            logFile = argv[++i];
        }
    }
    
    Server server(port, configFile, logFile);
    std::cout << "Starting server on port " << port << std::endl;
    std::cout << "User database: " << configFile << std::endl;
    std::cout << "Log file: " << logFile << std::endl;
    
    if (!server.start()) {
        std::cerr << "Failed to start server" << std::endl;
        return 1;
    }
    
    return 0;
}