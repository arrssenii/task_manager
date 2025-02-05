#include "Database.hpp"
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(65001); // Устанавливаем UTF-8 в консоли Windows
    Database db("task_manager.db");
    std::cout << "База данных успешно загружена!" << std::endl;
    return 0;
}
