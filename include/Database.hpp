#pragma once
#include "Task.hpp"

class Database {
public:
    Database(const std::string& dbPath);
    bool addTask(const Task& task);
    // Другие методы (удаление, обновление, получение списка)
};
