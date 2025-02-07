#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <string>
#include <vector>
#include "Task.hpp"

class Database {
private:
    sqlite3* db;
    std::string dbPath;

    void initialize();  // Внутренний метод для создания таблицы

public:
    Database(const std::string& path);
    ~Database();

    bool addTask(const std::string& title, const std::string& description, const std::string& deadline, const std::string& priority);
    std::vector<Task> getAllTasks();
    std::vector<Task> searchTasks(const std::string& keyword);
    std::vector<Task> filterTasks(const std::string& status = "", const std::string& priority = "", const std::string& deadline = "");
    void listTasks();
    bool updateTask(int id, const std::string& title, const std::string& description, const std::string& deadline, const std::string& priority, const std::string& status);
    bool deleteTask(int id);
};

#endif // DATABASE_HPP
