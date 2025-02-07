#include "Database.hpp"
#include <iostream>
#include <iomanip>

Database::Database(const std::string& path) : dbPath(path) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Ошибка при открытии БД: " << sqlite3_errmsg(db) << std::endl;
    } else {
        initialize();
    }
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::initialize() {
    const char* check_sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='tasks';";
    sqlite3_stmt* check_stmt;

    if (sqlite3_prepare_v2(db, check_sql, -1, &check_stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(check_stmt) == SQLITE_ROW) {
            std::cout << "Таблица 'tasks' уже существует." << std::endl;
            sqlite3_finalize(check_stmt);
            return;
        }
    }
    sqlite3_finalize(check_stmt);

    const char* sql = R"(
        CREATE TABLE tasks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP,
            deadline TEXT,
            priority TEXT CHECK(priority IN ('low', 'medium', 'high')),
            status TEXT CHECK(status IN ('active', 'completed', 'deleted'))
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "Ошибка при создании таблицы: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Таблица 'tasks' успешно создана!" << std::endl;
    }
}

bool Database::addTask(const std::string& title, const std::string& description, const std::string& deadline, const std::string& priority) {
    std::string sql = "INSERT INTO tasks (title, description, deadline, priority, status) VALUES (?, ?, ?, ?, 'active');";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, deadline.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, priority.c_str(), -1, SQLITE_STATIC);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (success) std::cout << "Задача успешно добавлена!" << std::endl;
    return success;
}

std::vector<Task> Database::getAllTasks() {
    std::vector<Task> tasks;
    const char* sql = "SELECT id, title, description, deadline, priority, status FROM tasks;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return tasks;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(stmt, 0);
        task.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        task.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        task.deadline = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        task.priority = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        task.status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

        tasks.push_back(task);
    }

    sqlite3_finalize(stmt);
    return tasks;
}

void Database::listTasks() {
    const char* sql = "SELECT id, title, description, deadline, priority, status FROM tasks;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    bool hasTasks = false;
    std::cout << std::left << std::setw(5) << "ID"
              << std::setw(20) << "Заголовок"
              << std::setw(30) << "Описание"
              << std::setw(15) << "Дедлайн"
              << std::setw(10) << "Приоритет"
              << std::setw(10) << "Статус"
              << std::endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        hasTasks = true;
        std::cout << std::left << std::setw(5) << sqlite3_column_int(stmt, 0)
                  << std::setw(20) << sqlite3_column_text(stmt, 1)
                  << std::setw(30) << sqlite3_column_text(stmt, 2)
                  << std::setw(15) << sqlite3_column_text(stmt, 3)
                  << std::setw(10) << sqlite3_column_text(stmt, 4)
                  << std::setw(10) << sqlite3_column_text(stmt, 5)
                  << std::endl;
    }

    if (!hasTasks) {
        std::cout << "Список задач пуст." << std::endl;
    }

    sqlite3_finalize(stmt);
}

bool Database::updateTask(int id, const std::string& title, const std::string& description, const std::string& deadline, const std::string& priority, const std::string& status) {
    std::string sql = "UPDATE tasks SET title = ?, description = ?, deadline = ?, priority = ?, status = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, deadline.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, priority.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, status.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (success) std::cout << "Задача успешно обновлена!" << std::endl;
    return success;
}

bool Database::deleteTask(int id) {
    std::string sql = "DELETE FROM tasks WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    bool success = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    if (success) std::cout << "Задача удалена!" << std::endl;
    return success;
}

std::vector<Task> Database::searchTasks(const std::string& keyword) {
    std::vector<Task> tasks;
    std::string sql = "SELECT id, title, description, deadline, priority, status FROM tasks "
                      "WHERE title LIKE ? OR description LIKE ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return tasks;
    }

    std::string searchPattern = "%" + keyword + "%";
    sqlite3_bind_text(stmt, 1, searchPattern.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, searchPattern.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(stmt, 0);
        task.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        task.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        task.deadline = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        task.priority = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        task.status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        tasks.push_back(task);
    }

    sqlite3_finalize(stmt);
    return tasks;
}

std::vector<Task> Database::filterTasks(const std::string& status, const std::string& priority, const std::string& deadline) {
    std::vector<Task> tasks;
    std::string query = "SELECT * FROM tasks WHERE 1=1"; // 1=1 нужно, чтобы удобно добавлять условия

    if (!status.empty()) {
        query += " AND status = ?";
    }
    if (!priority.empty()) {
        query += " AND priority = ?";
    }
    if (!deadline.empty()) {
        query += " AND deadline = ?";
    }

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Ошибка подготовки запроса: " << sqlite3_errmsg(db) << std::endl;
        return tasks;
    }

    int paramIndex = 1;
    if (!status.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, status.c_str(), -1, SQLITE_STATIC);
    }
    if (!priority.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, priority.c_str(), -1, SQLITE_STATIC);
    }
    if (!deadline.empty()) {
        sqlite3_bind_text(stmt, paramIndex++, deadline.c_str(), -1, SQLITE_STATIC);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Task task;
        task.id = sqlite3_column_int(stmt, 0);
        task.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        task.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        task.deadline = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        task.priority = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        task.status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        tasks.push_back(task);
    }

    sqlite3_finalize(stmt);
    return tasks;
}
