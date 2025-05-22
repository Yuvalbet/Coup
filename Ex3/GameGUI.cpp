#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.hpp"

void runGameGUI(Game& game) {
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "Coup Game Board");

    bool gameStarted = false;
    bool onStartScreen = true;
    bool choosingPlayerCount = false;
    bool enteringNames = false;
    int totalPlayers = 0;
    bool isInputActive = false;


    std::vector<std::string> playerNames;
    std::string currentInput = "";

    sf::Text inputText, titleText, startText, addPlayerText, playersTitle, playerNumberLabel;
    sf::RectangleShape startButton, addPlayerButton, nameInputBox;

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf")) {
        std::cerr << " Error loading font" << std::endl;
        return;
    }

    titleText.setFont(font);
    titleText.setString("The Coup Game");
    titleText.setCharacterSize(56);
    titleText.setFillColor(sf::Color(0, 102, 204));
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    titleText.setPosition(window.getSize().x / 2, 80);

    playersTitle.setFont(font);
    playersTitle.setString("Players:");
    playersTitle.setCharacterSize(32);
    playersTitle.setFillColor(sf::Color(80, 60, 150));
    playersTitle.setPosition(window.getSize().x / 2 - 200, 200);

    playerNumberLabel.setFont(font);
    playerNumberLabel.setCharacterSize(24);
    playerNumberLabel.setFillColor(sf::Color(50, 50, 50));
    playerNumberLabel.setPosition(window.getSize().x / 2 - 200, 260);

    nameInputBox.setSize(sf::Vector2f(400, 50));
    nameInputBox.setFillColor(sf::Color(235, 245, 250));
    nameInputBox.setOutlineColor(sf::Color(160, 160, 200));
    nameInputBox.setOutlineThickness(3);
    nameInputBox.setPosition(window.getSize().x / 2 - 200, 300);

    inputText.setFont(font);
    inputText.setCharacterSize(30);
    inputText.setFillColor(sf::Color(70, 70, 70));
    inputText.setPosition(nameInputBox.getPosition().x + 10, nameInputBox.getPosition().y + 10);

    addPlayerButton.setSize(sf::Vector2f(200, 50));
    addPlayerButton.setFillColor(sf::Color(180, 220, 200));
    addPlayerButton.setOutlineColor(sf::Color(100, 100, 100));
    addPlayerButton.setOutlineThickness(2);
    addPlayerButton.setPosition(window.getSize().x / 2 - 100, 380);
    
    addPlayerText.setFont(font);
    addPlayerText.setString("Add Player");
    addPlayerText.setCharacterSize(24);
    addPlayerText.setFillColor(sf::Color::Black);
    sf::FloatRect apTextBounds = addPlayerText.getLocalBounds();
    addPlayerText.setOrigin(apTextBounds.width / 2, apTextBounds.height / 2);
    addPlayerText.setPosition(addPlayerButton.getPosition().x + 100, addPlayerButton.getPosition().y + 25);

    startButton.setSize(sf::Vector2f(260, 60));
    startButton.setFillColor(sf::Color(180, 230, 200));  // Light green
    startButton.setOutlineColor(sf::Color(100, 100, 100));
    startButton.setOutlineThickness(2);
    startButton.setPosition(window.getSize().x / 2 - 130, 500);

    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(26);
    startText.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = startText.getLocalBounds();
    startText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    startText.setPosition(window.getSize().x / 2, 530);

    std::vector<sf::RectangleShape> playerCountButtons;
    std::vector<sf::Text> playerCountTexts;

    for (int i = 2; i <= 6; ++i) {
        sf::RectangleShape button(sf::Vector2f(60, 60));
        button.setFillColor(sf::Color(200, 220, 250));
        button.setOutlineColor(sf::Color(100, 100, 100));
        button.setOutlineThickness(2);
        button.setPosition(350 + (i - 2) * 100, 250);
        playerCountButtons.push_back(button);

        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(i));
        text.setCharacterSize(28);
        text.setFillColor(sf::Color::Black);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2, bounds.height / 2);
        text.setPosition(button.getPosition().x + 30, button.getPosition().y + 30);
        playerCountTexts.push_back(text);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (onStartScreen) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (startButton.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        onStartScreen = false;
                        choosingPlayerCount = true;
                    }
                }
            } else if (choosingPlayerCount) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < playerCountButtons.size(); ++i) {
                        if (playerCountButtons[i].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                            totalPlayers = i + 2;
                            choosingPlayerCount = false;
                            enteringNames = true;
                        }
                    }
                }
            } else if (enteringNames && !gameStarted) {
                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == 8) { // Backspace
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                            inputText.setString(currentInput);
                        }
                    }
                    else if (event.text.unicode < 128 && std::isprint(static_cast<char>(event.text.unicode))) {
                        currentInput += static_cast<char>(event.text.unicode);
                        inputText.setString(currentInput);
                    }
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (addPlayerButton.getGlobalBounds().contains(mousePos)) {
                        if (!currentInput.empty() && playerNames.size() < totalPlayers) {
                            playerNames.push_back(currentInput);
                            currentInput.clear();
                            inputText.setString("");
                        }
                    }
                    if (nameInputBox.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        isInputActive = true;
                    } else {
                        isInputActive = false;
                    }

                    if (startButton.getGlobalBounds().contains(mousePos)) {
                        if (playerNames.size() == totalPlayers) {
                            gameStarted = true;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(230, 230, 240));

        if (onStartScreen) {
            window.draw(titleText);

            window.draw(startButton);
            window.draw(startText);
        } else if (choosingPlayerCount) {
            sf::Text chooseText("Choose number of players:", font, 32);
            chooseText.setFillColor(sf::Color(60, 60, 60));
            sf::FloatRect ctBounds = chooseText.getLocalBounds();
    chooseText.setOrigin(ctBounds.width / 2, ctBounds.height / 2);
    
        sf::RectangleShape panel(sf::Vector2f(600, 300));
        panel.setFillColor(sf::Color(230, 240, 250));
        panel.setOutlineColor(sf::Color(180, 180, 200));
        panel.setOutlineThickness(3);
        panel.setPosition(window.getSize().x / 2 - 300, 120);
        window.draw(panel);

    chooseText.setPosition(window.getSize().x / 2, 210);
            window.draw(chooseText);
            for (const auto& b : playerCountButtons) window.draw(b);
            for (const auto& t : playerCountTexts) window.draw(t);
        } else if (enteringNames && !gameStarted) {
            window.draw(titleText);

            window.draw(playersTitle);
            window.draw(playerNumberLabel);
            window.draw(nameInputBox);
            sf::Text displayText = inputText;
            if (isInputActive)
                displayText.setString(currentInput + "|");
            else
                displayText.setString(currentInput);
            window.draw(displayText);
                        window.draw(addPlayerButton);
            window.draw(addPlayerText);

            float y = 460;
            for (const std::string& name : playerNames) {
                sf::Text nameText(name, font, 26);
                nameText.setFillColor(sf::Color(60, 60, 60));
                nameText.setPosition(window.getSize().x / 2 - 200, y);
                window.draw(nameText);
                y += 35;
            }

            if (playerNames.size() == totalPlayers) {
                window.draw(startButton);
                window.draw(startText);
            }
        }

        window.display();
    }
}