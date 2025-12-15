#include <UnitTest++/UnitTest++.h>
#include <fstream>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cstring>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

#define SERVER_TESTING
#include "server.h"

// Функция для создания временного файла с пользователями
string createTempUserDb(const vector<pair<string, string>>& users) {
    string filename = "temp_test_db_" + to_string(time(nullptr)) + ".txt";
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Cannot create temp file");
    }
    
    for (const auto& user_pair : users) {
        file << user_pair.first << ":" << user_pair.second << "\n";
    }
    file.close();
    return filename;
}

// Функция для удаления временного файла
void deleteTempFile(const string& filename) {
    remove(filename.c_str());
}

// ==================== ТЕСТЫ ВЫЧИСЛЕНИЙ ====================

SUITE(CalculationTest)
{
    TEST(CalculateProductPositiveNumbers) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        vector<int16_t> vector = {2, 3, 4};
        int16_t result = server.testCalculateProduct(vector);
        CHECK_EQUAL(24, result);
    }
    
    TEST(CalculateProductWithNegativeNumbers) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        vector<int16_t> vector = {-2, 3, -4};
        int16_t result = server.testCalculateProduct(vector);
        CHECK_EQUAL(24, result);
    }
    
    TEST(CalculateProductSingleElement) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        vector<int16_t> vector = {42};
        int16_t result = server.testCalculateProduct(vector);
        CHECK_EQUAL(42, result);
    }
    
    TEST(CalculateProductEmptyVector) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        vector<int16_t> vector;
        int16_t result = server.testCalculateProduct(vector);
        CHECK_EQUAL(0, result);
    }
    
    TEST(CalculateProductWithZero) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        vector<int16_t> vector = {1, 0, 5, 10};
        int16_t result = server.testCalculateProduct(vector);
        CHECK_EQUAL(0, result);
    }
    
    TEST(CalculateProductOverflowPositive) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        vector<int16_t> vector = {200, 200};
        int16_t result = server.testCalculateProduct(vector);
        CHECK_EQUAL(32767, result);
    }
    
    TEST(CalculateProductOverflowNegative) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        vector<int16_t> vector = {-200, 200};
        int16_t result = server.testCalculateProduct(vector);
        CHECK_EQUAL(-32768, result);
    }
}

// ==================== ТЕСТЫ ХЕШИРОВАНИЯ MD5 ====================

SUITE(MD5HashTest)
{
    TEST(MD5HashKnownValue1) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        string result = server.testMd5Hash("test123");
        CHECK_EQUAL("CC03E747A6AFBBCBF8BE7668ACFEBEE5", result);
    }
    
    TEST(MD5HashEmptyString) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        string result = server.testMd5Hash("");
        CHECK_EQUAL("D41D8CD98F00B204E9800998ECF8427E", result);
    }
    
    TEST(MD5HashKnownValue2) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        string result = server.testMd5Hash("hello");
        CHECK_EQUAL("5D41402ABC4B2A76B9719D911017C592", result);
    }
    
    TEST(MD5HashConsistency) {
        Server server(33333, "./vcalc.conf", "./log/vcalc.log");
        string input = "consistent_test_string_123";
        string hash1 = server.testMd5Hash(input);
        string hash2 = server.testMd5Hash(input);
        CHECK_EQUAL(hash1, hash2);
    }
}

// ==================== ТЕСТЫ ЗАГРУЗКИ БАЗЫ ПОЛЬЗОВАТЕЛЕЙ ====================

SUITE(UserDatabaseTest)
{
    TEST(LoadValidUserDatabase) {
        vector<pair<string, string>> users = {
            {"user1", "password123"},
            {"user2", "secret456"},
            {"admin", "adminpass"}
        };
        
        string filename = createTempUserDb(users);
        
        Server server(33333, filename, "./log/vcalc.log");
        server.testLoadUserDatabase();
        
        CHECK_EQUAL(3, server.getUsersCount());
        
        deleteTempFile(filename);
    }
    
    TEST(LoadEmptyDatabaseFile) {
        string filename = "empty_test_" + to_string(time(nullptr)) + ".txt";
        ofstream file(filename);
        file.close();
        
        Server server(33333, filename, "./log/vcalc.log");
        server.testLoadUserDatabase();
        
        CHECK_EQUAL(0, server.getUsersCount());
        
        deleteTempFile(filename);
    }
    
    TEST(LoadNonExistentDatabaseFile) {
        Server server(33333, "non_existent_file_12345.txt", "./log/vcalc.log");
        server.testLoadUserDatabase();
        CHECK_EQUAL(0, server.getUsersCount());
    }
}

// ==================== УПРОЩЕННЫЕ ТЕСТЫ ====================

SUITE(SimpleConstructorTest)
{
    TEST(ServerConstructorWorks) {
        // Просто проверяем, что конструктор не бросает исключений
        try {
            Server server(33333, "./vcalc.conf", "./log/vcalc.log");
            CHECK(true);
        } catch (...) {
            CHECK(false);
        }
    }
    
    TEST(ServerConstructorWithDifferentPorts) {
        try {
            Server server1(8080, "./vcalc.conf", "./log/vcalc.log");
            Server server2(9000, "./vcalc.conf", "./log/vcalc.log");
            CHECK(true);
        } catch (...) {
            CHECK(false);
        }
    }
}

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================

int main()
{
    cout << "Starting server tests..." << endl;
    cout << "=========================" << endl;
    
    int result = UnitTest::RunAllTests();
    
    cout << "=========================" << endl;
    cout << "Tests completed." << endl;
    
    // Очистка временных файлов
    system("rm -f temp_test_db_*.txt 2>/dev/null || true");
    system("rm -f empty_test_*.txt 2>/dev/null || true");
    
    return result;
}