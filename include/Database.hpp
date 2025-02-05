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

    void createTable();  // Внутренний метод для создания таблицы

public:
    Database(const std::string& path);
    ~Database();

    bool addTask(const Task& task);
    std::vector<Task> getAllTasks();
    bool updateTask(int id, bool completed, const std::string& newDescription);
    bool deleteTask(int id);
};

#endif // DATABASE_HPP
