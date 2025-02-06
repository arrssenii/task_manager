#include <iostream>
#include <string>
#include <windows.h>  // Для SetConsoleOutputCP
#include "Database.hpp"

void printMenu() {
    std::cout << "\nДоступные команды:\n"
              << "  add       - Добавить задачу\n"
              << "  list      - Показать список задач\n"
              << "  update    - Обновить задачу\n"
              << "  delete    - Удалить задачу\n"
              << "  exit      - Выйти из программы\n";
}

int main() {
    // Устанавливаем кодировку UTF-8 для корректного отображения
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Database db("tasks.db");
    std::string command;

    std::cout << "Добро пожаловать в Task Manager!" << std::endl;
    printMenu();

    while (true) {
        std::cout << "\nВведите команду: ";
        std::cin >> command;

        if (command == "add") {
            std::string title, description, deadline, priority;
            std::cout << "Название: ";
            std::cin.ignore();
            std::getline(std::cin, title);
            std::cout << "Описание: ";
            std::getline(std::cin, description);
            std::cout << "Дедлайн (YYYY-MM-DD): ";
            std::getline(std::cin, deadline);
            std::cout << "Приоритет (low, medium, high): ";
            std::getline(std::cin, priority);

            db.addTask(title, description, deadline, priority);
        }
        else if (command == "list") {
            db.listTasks();
        }
        else if (command == "update") {
            int id;
            std::string title, description, deadline, priority, status;
            std::cout << "ID задачи для обновления: ";
            std::cin >> id;
            std::cin.ignore();
            std::cout << "Новое название: ";
            std::getline(std::cin, title);
            std::cout << "Новое описание: ";
            std::getline(std::cin, description);
            std::cout << "Новый дедлайн (YYYY-MM-DD): ";
            std::getline(std::cin, deadline);
            std::cout << "Новый приоритет (low, medium, high): ";
            std::getline(std::cin, priority);
            std::cout << "Новый статус (active, completed, deleted): ";
            std::getline(std::cin, status);

            db.updateTask(id, title, description, deadline, priority, status);
        }
        else if (command == "delete") {
            int id;
            std::cout << "ID задачи для удаления: ";
            std::cin >> id;
            db.deleteTask(id);
        }
        else if (command == "exit") {
            std::cout << "Выход из программы...\n";
            break;
        }
        else {
            std::cout << "Неизвестная команда. Попробуйте снова.";
            printMenu();
        }
    }

    return 0;
}
