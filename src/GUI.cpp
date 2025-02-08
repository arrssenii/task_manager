#include "GUI.hpp"
#include <iostream>

GUI::GUI(Database& db) : db(db), window(sf::VideoMode(800, 600), "Task Manager") {
    setupUI();
}

void GUI::setupUI() {
    if (!font.loadFromFile("NotoSans-Regular.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
    }

    titleText.setFont(font);
    titleText.setString("Task Manager");
    titleText.setCharacterSize(24);
    titleText.setPosition(300, 20);
    titleText.setFillColor(sf::Color::White);

    addButton.setSize(sf::Vector2f(200, 50));
    addButton.setPosition(300, 100);
    addButton.setFillColor(sf::Color::Blue);

    addButtonText.setFont(font);
    addButtonText.setString("Add task");
    addButtonText.setCharacterSize(18);
    addButtonText.setPosition(320, 115);
    addButtonText.setFillColor(sf::Color::White);

    listButton.setSize(sf::Vector2f(200, 50));
    listButton.setPosition(300, 180);
    listButton.setFillColor(sf::Color::Blue);

    listButtonText.setFont(font);
    listButtonText.setString("Show tasks");
    listButtonText.setCharacterSize(18);
    listButtonText.setPosition(320, 195);
    listButtonText.setFillColor(sf::Color::White);

    deleteButton.setSize(sf::Vector2f(200, 50));
    deleteButton.setPosition(300, 260);
    deleteButton.setFillColor(sf::Color::Blue);

    deleteButtonText.setFont(font);
    deleteButtonText.setString("Delete task");
    deleteButtonText.setCharacterSize(18);
    deleteButtonText.setPosition(320, 275);
    deleteButtonText.setFillColor(sf::Color::White);

    exitButton.setSize(sf::Vector2f(200, 50));
    exitButton.setPosition(300, 340);
    exitButton.setFillColor(sf::Color::Red);

    exitButtonText.setFont(font);
    exitButtonText.setString("Exit");
    exitButtonText.setCharacterSize(18);
    exitButtonText.setPosition(370, 355);
    exitButtonText.setFillColor(sf::Color::White);
}

void GUI::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

void GUI::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (addButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                openAddTaskWindow();
            } else if (listButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                openTaskListWindow();
            } else if (deleteButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                openDeleteTaskWindow();
            } else if (exitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                window.close();
            }
        }
    }
}


void GUI::render() {
    window.clear();
    window.draw(titleText);
    window.draw(addButton);
    window.draw(addButtonText);
    window.draw(listButton);
    window.draw(listButtonText);
    window.draw(deleteButton);
    window.draw(deleteButtonText);
    window.draw(exitButton);
    window.draw(exitButtonText);
    window.display();
}

void GUI::openAddTaskWindow() {
    sf::RenderWindow addTaskWindow(sf::VideoMode(400, 300), "Add Task");

    sf::Font font;
    if (!font.loadFromFile("NotoSans-Regular.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
        return;
    }

    // Поля ввода
    sf::RectangleShape inputBox(sf::Vector2f(250, 30));
    inputBox.setFillColor(sf::Color(200, 200, 200));
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);

    sf::Text inputText("", font, 16);
    inputText.setFillColor(sf::Color::Black);

    std::vector<std::string> inputs(4, "");
    std::vector<std::string> labels = {"Title:", "Description:", "Priority:", "Status:"};
    int selectedField = 0; // Какое поле редактируется

    sf::RectangleShape saveButton(sf::Vector2f(100, 30));
    saveButton.setPosition(150, 250);
    saveButton.setFillColor(sf::Color::Green);

    sf::Text saveText("Save", font, 16);
    saveText.setPosition(175, 255);
    saveText.setFillColor(sf::Color::White);

    while (addTaskWindow.isOpen()) {
        sf::Event event;
        while (addTaskWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                addTaskWindow.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // Backspace
                    if (!inputs[selectedField].empty()) {
                        inputs[selectedField].pop_back();
                    }
                } else if (event.text.unicode == 13) { // Enter
                    selectedField = (selectedField + 1) % 4;
                } else if (event.text.unicode < 128) {
                    inputs[selectedField] += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(addTaskWindow);
                if (saveButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    db.addTask(inputs[0], inputs[1], inputs[2], inputs[3]);
                    addTaskWindow.close();
                }
            }
        }

        addTaskWindow.clear();
        for (int i = 0; i < 4; i++) {
            sf::Text label(labels[i], font, 16);
            label.setPosition(20, 20 + i * 50);
            label.setFillColor(sf::Color::White);
            addTaskWindow.draw(label);

            inputBox.setPosition(120, 20 + i * 50);
            if (selectedField == i) {
                inputBox.setOutlineColor(sf::Color::Red);
            } else {
                inputBox.setOutlineColor(sf::Color::Black);
            }
            addTaskWindow.draw(inputBox);

            inputText.setString(inputs[i]);
            inputText.setPosition(125, 25 + i * 50);
            addTaskWindow.draw(inputText);
        }

        addTaskWindow.draw(saveButton);
        addTaskWindow.draw(saveText);
        addTaskWindow.display();
    }
}

void GUI::openTaskListWindow() {
    sf::RenderWindow taskListWindow(sf::VideoMode(600, 400), "Task List");

    sf::Font font;
    if (!font.loadFromFile("NotoSans-Regular.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
        return;
    }

    std::vector<Task> tasks = db.getAllTasks();
    std::vector<sf::Text> taskTexts;

    int yOffset = 50;
    for (const auto& task : tasks) {
        sf::Text taskText(task.title + " | " + task.priority + " | " + task.status, font, 16);
        taskText.setPosition(20, yOffset);
        taskText.setFillColor(sf::Color::White);
        taskTexts.push_back(taskText);
        yOffset += 30;
    }

    while (taskListWindow.isOpen()) {
        sf::Event event;
        while (taskListWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                taskListWindow.close();
            }
        }

        taskListWindow.clear();
        for (auto& text : taskTexts) {
            taskListWindow.draw(text);
        }
        taskListWindow.display();
    }
}

void GUI::openDeleteTaskWindow() {
    std::cout << "Открытие окна удаления задачи...\n";
}
