#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Fade Animation Example");

    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(100, 100);

    sf::Clock clock;

    float alpha = 255; // Начальная непрозрачность
    bool fading = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Анимация исчезновения
        if (fading) {
            alpha -= 50 * clock.restart().asSeconds(); // Уменьшаем альфа-канал
            if (alpha <= 0) {
                alpha = 0;
                fading = false; // Останавливаем анимацию
            }
        }

        // Обновляем цвет объекта с новой прозрачностью
        shape.setFillColor(sf::Color(0, 255, 0, static_cast<sf::Uint8>(alpha)));

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
