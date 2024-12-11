#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

enum class GameState { Menu, Playing, Paused, GameOver };

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Lesson 6: Game States with Pause and Transitions");

    // Background setup
    sf::Texture backgroundTexture;
    sf::Sprite background;
    sf::Vector2u textureSize, windowSize;

    if (!backgroundTexture.loadFromFile("msu_texas_background.jpeg")) {
        std::cerr << "Error loading background image\n";
        return -1;
    } else {
        // Scale background to fit the window size
        textureSize = backgroundTexture.getSize();
        windowSize = window.getSize();
        float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
        float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
        background.setTexture(backgroundTexture);
        background.setScale(scaleX, scaleY);
    }

    // Current game state
    GameState currentState = GameState::Menu;

    // Font and text
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }
    sf::Text menuText("Press Enter to Play", font, 50);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition(250.f, 250.f);

    sf::Text gameOverText("Game Over! Press R to Restart", font, 50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(150.f, 250.f);

    sf::Text pausedText("Paused! Press R to Resume", font, 50);
    pausedText.setFillColor(sf::Color::Blue);
    pausedText.setPosition(200.f, 250.f);

    // Click counter setup
    int clickCount = 0;
    sf::Text clickCounterText;
    clickCounterText.setFont(font);
    clickCounterText.setCharacterSize(30);
    clickCounterText.setFillColor(sf::Color::White);
    clickCounterText.setPosition(10.f, 10.f);
    clickCounterText.setString("Clicks: 0");

    // Player setup
    sf::CircleShape player(50.f);
    player.setFillColor(sf::Color::Blue);
    player.setPosition(300.f, 300.f);

    float playerSpeed = 5.0f;

    // Clock for transitions
    sf::Clock clock;
    sf::RectangleShape fadeEffect(sf::Vector2f(800.f, 600.f));
    fadeEffect.setFillColor(sf::Color(0, 0, 0, 0));

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // State-specific input handling
            if (currentState == GameState::Menu) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    currentState = GameState::Playing;
                    clock.restart(); // Start fade-in effect
                }
            } else if (currentState == GameState::Playing) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    currentState = GameState::GameOver;
                } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
                    currentState = GameState::Paused;
                }

                // Player movement
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player.move(0, -playerSpeed);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.move(-playerSpeed, 0);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player.move(0, playerSpeed);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.move(playerSpeed, 0);

                // Check for mouse clicks on the player circle
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (player.getGlobalBounds().contains(mousePos)) {
                        clickCount++;
                        clickCounterText.setString("Clicks: " + std::to_string(clickCount));
                    }
                }

            } else if (currentState == GameState::Paused) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    currentState = GameState::Playing;
                }
            } else if (currentState == GameState::GameOver) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    currentState = GameState::Menu;
                    clickCount = 0; // Reset click counter
                    clickCounterText.setString("Clicks: 0");
                }
            }
        }


        // Draw background
        window.draw(background);

        if (currentState == GameState::Menu) {
            window.draw(menuText);
        } else if (currentState == GameState::Playing) {
            // Fade-in effect
            sf::Time elapsed = clock.getElapsedTime();
            if (elapsed.asSeconds() < 1.0f) {
                int alpha = static_cast<int>((1.0f - elapsed.asSeconds()) * 255);
                fadeEffect.setFillColor(sf::Color(0, 0, 0, alpha));
                window.draw(fadeEffect);
            }

            window.draw(player);
            window.draw(clickCounterText);
        } else if (currentState == GameState::Paused) {
            window.draw(player);
            window.draw(pausedText);
            window.draw(clickCounterText);
        } else if (currentState == GameState::GameOver) {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
