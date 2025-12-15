/**
 * @file server.h
 * @author Полежаев А.И.
 * @date 15.12.2025
 * @brief Заголовочный файл класса Server.
 * @details Объявление класса сервера для обработки сетевых подключений,
 *          аутентификации клиентов и вычисления произведений векторов.
 */

#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

/**
 * @brief Класс сервера для обработки клиентских подключений.
 * @details Обеспечивает сетевую коммуникацию, аутентификацию пользователей
 *          по MD5-хэшу, прием и обработку векторных данных, а также логирование.
 */
class Server {
public:
    /**
     * @brief Конструктор сервера.
     * @param port Порт для прослушивания подключений.
     * @param userDbPath Путь к файлу базы данных пользователей.
     * @param logPath Путь к файлу журнала сервера.
     */
    Server(int port, const std::string& userDbPath, const std::string& logPath);
    
    /**
     * @brief Запускает сервер и начинает прослушивание порта.
     * @return true если сервер успешно запущен, false при ошибке.
     */
    bool start();

private:
    int port;                                       ///< Порт сервера
    std::string userDbPath;                         ///< Путь к базе пользователей
    std::string logPath;                            ///< Путь к файлу журнала
    std::unordered_map<std::string, std::string> users; ///< Кэш пользователей
    
    /**
     * @brief Записывает сообщение об ошибке в журнал.
     * @param message Текст сообщения.
     * @param isCritical Флаг критичности ошибки.
     */
    void logError(const std::string& message, bool isCritical);
    
    /**
     * @brief Загружает базу данных пользователей из файла.
     */
    void loadUserDatabase();
    
    /**
     * @brief Вычисляет MD5-хэш строки.
     * @param input Входная строка.
     * @return MD5-хэш в шестнадцатеричном формате.
     */
    std::string md5Hash(const std::string& input);
    
    /**
     * @brief Обрабатывает подключение клиента.
     * @param clientSocket Дескриптор сокета клиента.
     */
    void handleClient(int clientSocket);
    
    /**
     * @brief Аутентифицирует клиента.
     * @param clientSocket Дескриптор сокета клиента.
     * @return true если аутентификация успешна.
     */
    bool authenticate(int clientSocket);
    
    /**
     * @brief Обрабатывает передачу векторов от клиента.
     * @param clientSocket Дескриптор сокета клиента.
     */
    void processVectors(int clientSocket);
    
    /**
     * @brief Вычисляет произведение элементов вектора.
     * @param vector Вектор целых чисел.
     * @return Произведение элементов вектора.
     */
    int16_t calculateProduct(const std::vector<int16_t>& vector);
    
    #ifdef SERVER_TESTING
    public:
        /**
         * @brief Возвращает количество загруженных пользователей.
         * @return Количество пользователей в базе.
         */
        size_t getUsersCount() const { return users.size(); }
        
        /**
         * @brief Тестовый метод для вычисления произведения вектора.
         * @param vector Вектор для обработки.
         * @return Произведение элементов вектора.
         */
        int16_t testCalculateProduct(const std::vector<int16_t>& vector) {
            return calculateProduct(vector);
        }
        
        /**
         * @brief Тестовый метод для вычисления MD5-хэша.
         * @param input Строка для хэширования.
         * @return MD5-хэш строки.
         */
        std::string testMd5Hash(const std::string& input) {
            return md5Hash(input);
        }
        
        /**
         * @brief Тестовый метод для загрузки базы данных пользователей.
         */
        void testLoadUserDatabase() {
            loadUserDatabase();
        }
    #endif
};

#endif // SERVER_H