#include <cmath>      // בשביל cos, sin
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.hpp"
#include "Roles/Spy.hpp"


void runGameGUI(Game& game) {
    
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "Coup Game Board");

    bool gameStarted = false;
    bool showMainGameScreen = false;
    bool onStartScreen = true;
    bool choosingPlayerCount = false;
    bool enteringNames = false;
    int totalPlayers = 0;
    bool isInputActive = false;
    bool awaitingTarget = false;
    int pendingAction = 0; // 4 = Arrest, 5 = Sanction, 6 = Coup, 9 = Spy

    
    std::vector<sf::RectangleShape> actionButtons;
    std::vector<std::string> actionLabels;

    std::vector<std::string> playerNames;
    std::string currentInput = "";

    sf::Text inputText, titleText, startText, addPlayerText, playersTitle, playerNumberLabel, errorText;
    sf::RectangleShape startButton, addPlayerButton, nameInputBox;

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf")) {
        std::cerr << " Error loading font" << std::endl;
        return;
    }

    errorText.setFont(font);
    errorText.setCharacterSize(22);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(window.getSize().x / 2 - 200, 430); // מתחת לכפתור Add Player
    errorText.setString(""); // מתחיל ריק


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
    startButton.setPosition(window.getSize().x / 2 - 130, 580);

    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(26);
    startText.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = startText.getLocalBounds();
    startText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    startText.setPosition(window.getSize().x / 2, 610);

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
                            bool alreadyExists = false;
                            for (const std::string& name : playerNames) {
                                if (name == currentInput) {
                                    alreadyExists = true;
                                    break;
                                }
                            }

                            if (alreadyExists) {
                                errorText.setString("Name already exists. Please choose a different name.");
                            } else {
                                playerNames.push_back(currentInput);
                                currentInput.clear();
                                inputText.setString("");
                                errorText.setString(""); // מנקה את השגיאה אם הייתה
                            }
                        }

                    }
                    if (nameInputBox.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                        isInputActive = true;
                    } else {
                        isInputActive = false;
                    }

                    if (startButton.getGlobalBounds().contains(mousePos)) {
                        if (playerNames.size() == totalPlayers) {
                            game.assignRandomRoles(playerNames);  // ⬅️ השורה החדשה
                            gameStarted = true;
                            game.setCurrentTurnIndex(0);  // מתחילים מהשחקן הראשון
                        }
                    }

                }
            }

          else if (gameStarted && !showMainGameScreen) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        float y = 180 + game.getPlayers().size() * 60; // ← תואם לגובה 50 + ריווח 10

        sf::FloatRect startTurnBounds(
            window.getSize().x / 2 - 150,
            y + 40,
            300,
            70
        );

        if (startTurnBounds.contains(mousePos)) {
            showMainGameScreen = true;
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
                window.draw(errorText);
                window.draw(addPlayerText);

            float y = 460;
            for (const std::string& name : playerNames) {
                sf::Text nameText(name, font, 26);
                nameText.setFillColor(sf::Color(60, 60, 60));
                nameText.setPosition(window.getSize().x / 2 - 200, y);
                window.draw(nameText);
                y += 30;
            }

            if (playerNames.size() == totalPlayers) {
                // מקם את הכפתור אחרי השם האחרון
                startButton.setPosition(window.getSize().x / 2 - 130, y + 20);
                startText.setPosition(window.getSize().x / 2, y + 20 + 30); // מיקום טקסט על הכפתור

                window.draw(startButton);
                window.draw(startText);
            }

        }else if (gameStarted && !showMainGameScreen) {
            window.clear(sf::Color(240, 245, 250));


            sf::Text title("Players and Roles", font, 46);
            title.setFillColor(sf::Color(40, 40, 90));
            title.setStyle(sf::Text::Bold | sf::Text::Underlined);
            sf::FloatRect titleBounds = title.getLocalBounds();
            title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
            title.setPosition(window.getSize().x / 2, 80);
            window.draw(title);

            float y = 180;
            for (Player* p : game.getPlayers()) {
                sf::RectangleShape card(sf::Vector2f(600, 50));
                card.setFillColor(sf::Color(255, 255, 255));
                card.setOutlineColor(sf::Color(180, 180, 200));
                card.setOutlineThickness(2);
                card.setPosition(window.getSize().x / 2 - 300, y);
                window.draw(card);

                sf::Text nameText(p->getName(), font, 26);
                nameText.setFillColor(sf::Color(50, 50, 50));
                nameText.setPosition(card.getPosition().x + 20, y + 15);
                window.draw(nameText);

                sf::Text roleText(p->getRoleName(), font, 26);
                roleText.setFillColor(sf::Color(80, 50, 120));
                sf::FloatRect roleBounds = roleText.getLocalBounds();
                roleText.setOrigin(roleBounds.width, 0);
                roleText.setPosition(card.getPosition().x + 580, y + 15);
                window.draw(roleText);

                y += 60;

                
            }

    sf::RectangleShape startTurnButton(sf::Vector2f(300, 70));
    startTurnButton.setFillColor(sf::Color(180, 230, 200));
    startTurnButton.setOutlineColor(sf::Color(100, 100, 100));
    startTurnButton.setOutlineThickness(2);
    startTurnButton.setPosition(window.getSize().x / 2 - 150, y + 40); // יושב קצת יותר גבוה שלא ייחתך
    window.draw(startTurnButton);

    sf::Text startTurnText("Start", font, 30);
    startTurnText.setFillColor(sf::Color::Black);
    sf::FloatRect stBounds = startTurnText.getLocalBounds();
    startTurnText.setOrigin(stBounds.width / 2, stBounds.height / 2);
    startTurnText.setPosition(startTurnButton.getPosition().x + 150, startTurnButton.getPosition().y + 35);
    window.draw(startTurnText);

    

} else if (gameStarted && showMainGameScreen) {
    
        window.clear(sf::Color(230, 240, 250));

        sf::Vector2f center(window.getSize().x / 2, window.getSize().y / 2);
        float radius = 300.0f;

        const std::vector<Player*>& players = game.getPlayers();
        size_t playerCount = players.size();

        sf::CircleShape tableCircle(radius);
        tableCircle.setFillColor(sf::Color(200, 220, 240));
        tableCircle.setOrigin(radius, radius);
        tableCircle.setPosition(center);
        window.draw(tableCircle);

       for (size_t i = 0; i < playerCount; ++i) {
        float angle = 2 * M_PI * i / playerCount;
        float x = center.x + radius * std::cos(angle);
        float y = center.y + radius * std::sin(angle);

        // כרטיס שחקן
        sf::RectangleShape card(sf::Vector2f(150, 60));
        card.setFillColor(sf::Color::White);
        card.setOutlineColor(sf::Color::Black);
        card.setOutlineThickness(2);
        card.setOrigin(75, 30);
        card.setPosition(x, y);
            
        if (i == game.getCurrentTurnIndex()) {
            card.setFillColor(sf::Color(180, 255, 180));  // ירוק בהיר לשחקן בתורו
        } else {
            card.setFillColor(sf::Color::White);
        }

        window.draw(card);

        // שם
        sf::Text nameText(players[i]->getName(), font, 20);
        nameText.setFillColor(sf::Color::Black);
        nameText.setOrigin(nameText.getLocalBounds().width / 2, nameText.getLocalBounds().height / 2);
        nameText.setPosition(x, y - 10);
        window.draw(nameText);

        // תפקיד
        sf::Text roleText(players[i]->getRoleName(), font, 16);
        roleText.setFillColor(sf::Color(100, 100, 160));
        roleText.setOrigin(roleText.getLocalBounds().width / 2, roleText.getLocalBounds().height / 2);
        roleText.setPosition(x, y + 15);
        window.draw(roleText);

        // רק לשחקן שתורו נציג כפתורי פעולות
        if (i == game.getCurrentTurnIndex()) {
            Player* current = players[i];

            // מציג את שם השחקן ומספר המטבעות
            std::string infoText = "Turn: " + current->getName() + " | Coins: " + std::to_string(current->getCoins());
            sf::Text info(infoText, font, 22);
            info.setFillColor(sf::Color(40, 40, 40));
            info.setPosition(window.getSize().x - 400, 100);  // בצד ימין למעלה
            window.draw(info);

            std::vector<std::string> actions = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup"};

            std::string role = current->getRoleName();

        
            // אל תוסיף כאן פעולות כמו "Block Coup" או "Block Tax" — הן יופעלו בזמן אמת מתוך Game.cpp

            if (role == "Spy") actions.push_back("Spy Action");
            

            actionButtons.clear();
            actionLabels.clear();

            float bx = window.getSize().x - 220;
            float by = 150;
            for (const std::string& act : actions) {
                sf::RectangleShape btn(sf::Vector2f(180, 40));
                btn.setFillColor(sf::Color(255, 255, 200));
                btn.setOutlineColor(sf::Color::Black);
                btn.setOutlineThickness(2);
                btn.setPosition(bx, by);

                actionButtons.push_back(btn);
                actionLabels.push_back(act);

                window.draw(btn);

                sf::Text text(act, font, 18);
                text.setFillColor(sf::Color::Black);
                text.setPosition(bx + 10, by + 7);
                window.draw(text);

                by += 50;
            }

                    }


                
                }

                }
              // ✅ תוספת חדשה (צעד 2) - חיבור כפתורי הפעולה למימושים האמיתיים
if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

    for (size_t j = 0; j < actionButtons.size(); ++j) {
        if (actionButtons[j].getGlobalBounds().contains(mousePos)) {
            std::string action = actionLabels[j];
            Player* current = game.currentPlayer();
            try {
                if (action == "Gather") {
                    current->gather();
                    game.nextTurn();  // תור עובר מיד
                }
                else if (action == "Tax") {
                    current->tax();
                    game.nextTurn();  // תור עובר מיד
                }
                else if (action == "Bribe") {
                    current->bribe();
                    game.nextTurn();  // תור עובר מיד
                }
                else if (action == "Arrest") {
                    awaitingTarget = true;
                    pendingAction = 4;
                }
                else if (action == "Sanction") {
                    awaitingTarget = true;
                    pendingAction = 5;
                }
                else if (action == "Coup") {
                    awaitingTarget = true;
                    pendingAction = 6;
                }
                else if (action == "Spy Action") {
                    Spy* spy = dynamic_cast<Spy*>(current);
                    if (spy) {
                        awaitingTarget = true;
                        pendingAction = 9;
                    }
                }

            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
                // כאן אפשר להוסיף טקסט שגיאה במסך
            }
        }
    }
}



                window.display();
                }
            }