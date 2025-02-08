#include <iostream>
#include <string>
#include <windows.h>  // Для SetConsoleOutputCP
#include "GUI.hpp"
#include "Database.hpp"

void printMenu() {
    std::cout << "\nДоступные команды:\n"
              << "  add       - Добавить задачу\n"
              << "  list      - Показать список задач\n"
              << "  update    - Обновить задачу\n"
              << "  delete    - Удалить задачу\n"
              << "  search    - Поиск задач по ключевому слову\n"
              << "  filter    - Фильтрация задач по статусу или приоритету\n"
              << "  exit      - Выйти из программы\n";
}

int main() {
    // Устанавливаем кодировку UTF-8 для корректного отображения
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Database db("tasks.db");
    std::string command;

    GUI gui(db);
    gui.run();

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
        else if (command == "search") {
            std::cin.ignore(); // Очистка буфера перед вводом строки
            std::string keyword;
            std::cout << "Введите ключевое слово для поиска: ";
            std::getline(std::cin, keyword);

            std::vector<Task> tasks = db.searchTasks(keyword);
            if (tasks.empty()) {
                std::cout << "Задачи не найдены.\n";
            } else {
                std::cout << "ID   Заголовок  Описание  Дедлайн  Приоритет  Статус\n";
                for (const auto& task : tasks) {
                    std::cout << task.id << "  " << task.title << "  "
                              << task.description << "  " << task.deadline << "  "
                              << task.priority << "  " << task.status << "\n";
                }
            }
        }
        else if (command == "filter") {
            std::cin.ignore(); // Очистка буфера перед вводом
            std::string status, priority, deadline;

            std::cout << "Введите статус (или оставьте пустым): ";
            std::getline(std::cin, status);

            std::cout << "Введите приоритет (или оставьте пустым): ";
            std::getline(std::cin, priority);

            std::cout << "Введите дедлайн (или оставьте пустым): ";
            std::getline(std::cin, deadline);

            std::vector<Task> tasks = db.filterTasks(status, priority, deadline);

            if (tasks.empty()) {
                std::cout << "Задачи не найдены по заданным параметрам.\n";
            } else {
                std::cout << "ID   Заголовок  Описание  Дедлайн  Приоритет  Статус\n";
                for (const auto& task : tasks) {
                    std::cout << task.id << "  " << task.title << "  "
                              << task.description << "  " << task.deadline << "  "
                              << task.priority << "  " << task.status << "\n";
                }
            }
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
