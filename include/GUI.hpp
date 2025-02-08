#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include "Database.hpp"

class GUI {
public:
    GUI(Database& db);
    void run();

private:
    void handleEvents();
    void render();
    void setupUI();
    void openAddTaskWindow();
    void openTaskListWindow();
    void openDeleteTaskWindow();


    Database& db;
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::RectangleShape addButton;
    sf::RectangleShape listButton;
    sf::RectangleShape deleteButton;
    sf::RectangleShape exitButton;
    sf::Text addButtonText;
    sf::Text listButtonText;
    sf::Text deleteButtonText;
    sf::Text exitButtonText;
};

#endif // GUI_HPP
