#include "Database.hpp"
#include <iostream>

Database::Database(const std::string& path) : dbPath(path) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Ошибка при открытии БД: " << sqlite3_errmsg(db) << std::endl;
    } else {
        createTable();
    }
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::createTable() {
    const char* sql = "CREATE TABLE IF NOT EXISTS tasks ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "title TEXT NOT NULL, "
                      "description TEXT, "
                      "completed BOOLEAN NOT NULL DEFAULT 0);";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Ошибка при создании таблицы: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}
