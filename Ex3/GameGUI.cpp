#include <cmath> // בשביל cos, sin
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.hpp"
#include "Roles/Spy.hpp"
#include "Player.hpp"
#include "Roles/Baron.hpp"


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
    sf::Text messageText;
    std::string displayMessage = "";
    
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf")) {
        std::cerr << " Error loading font" << std::endl;
        return;
    }
    
    messageText.setFont(font);
    messageText.setCharacterSize(22);
    messageText.setFillColor(sf::Color(30, 30, 120));
    messageText.setPosition(20, 20);
    
    errorText.setFont(font);
    errorText.setCharacterSize(22);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(window.getSize().x / 2 - 200, 430);
    errorText.setString("");
    
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
    startButton.setFillColor(sf::Color(180, 230, 200));
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
                    for (size_t i = 0; i < playerCountButtons.size(); ++i) {
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
                                errorText.setString("");
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
                            game.assignRandomRoles(playerNames);
                            for (Player* p : game.getPlayers()) {
                                while (p->getCoins() > 0)
                                    p->removeCoins(1);
                            }
                            gameStarted = true;
                            game.setCurrentTurnIndex(0);
                        }
                    }
                }
            }
            else if (gameStarted && !showMainGameScreen) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    float y = 180 + game.getPlayers().size() * 60;
                    
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
            // FIXED: Move the main game screen event handling here, OUTSIDE the drawing loop
            else if (gameStarted && showMainGameScreen) {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    
                    // If awaiting target selection
                    if (awaitingTarget) {
                        // Check if clicking on a player card to select as target
                        sf::Vector2f center(window.getSize().x / 2, window.getSize().y / 2);
                        float radius = 300.0f;
                        const std::vector<Player*>& players = game.getPlayers();
                        size_t playerCount = players.size();
                        
                        int indexOnCircle = 0;
                        for (size_t j = 0; j < players.size(); ++j) {
                            Player* player = players[j];
                            if (!player->isActive()) continue;
                            
                            // Skip current player for target selection
                            if (j == game.getCurrentTurnIndex()) {
                                indexOnCircle++;
                                continue;
                            }
                            
                            float angle = 2 * M_PI * indexOnCircle / static_cast<float>(playerCount);
                            float x = center.x + radius * std::cos(angle);
                            float y = center.y + radius * std::sin(angle);
                            indexOnCircle++;
                            
                            sf::FloatRect playerCard(x - 75, y - 30, 150, 60);
                            if (playerCard.contains(mousePos)) {
                                Player* current = game.currentPlayer();
                                try {
                                    if (pendingAction == 4) { // Arrest
                                        game.playTurn(4, player);
                                        displayMessage = game.getLastActionMessage();
                                        
                                    } else if (pendingAction == 5) { // Sanction
                                        game.playTurn(5, player);
                                        displayMessage = game.getLastActionMessage();
                                        
                                    } if (pendingAction == 6) { // Coup
                                        Player* performer = game.currentPlayer();
                                        Player* target = player;

                                        if (performer->getCoins() < 7) {
                                            displayMessage = performer->getName() + " does not have enough coins to perform a coup.";
                                        } else {
                                            performer->removeCoins(7); // משלם מיד

                                            bool blocked = false;

                                            for (Player* p : game.getPlayers()) {
                                                if (p == performer || !p->isActive()) continue;

                                                if (General* general = dynamic_cast<General*>(p)) {
                                                    if (general->getCoins() < 5) continue;

                                                    // Popup לשאול אם לחסום
                                                    sf::RenderWindow popup(sf::VideoMode(400, 200), "Block Coup?");
                                                    sf::Text msg("Block coup of " + performer->getName() + " on " + target->getName() + "?", font, 20);
                                                    msg.setPosition(30, 50);
                                                    msg.setFillColor(sf::Color(50, 50, 120));

                                                    sf::RectangleShape yesBtn(sf::Vector2f(100, 40));
                                                    yesBtn.setPosition(70, 120);
                                                    yesBtn.setFillColor(sf::Color(180, 240, 200));

                                                    sf::RectangleShape noBtn(sf::Vector2f(100, 40));
                                                    noBtn.setPosition(230, 120);
                                                    noBtn.setFillColor(sf::Color(250, 180, 180));

                                                    sf::Text yesText("Yes", font, 20);
                                                    yesText.setPosition(100, 125);
                                                    yesText.setFillColor(sf::Color::Black);

                                                    sf::Text noText("No", font, 20);
                                                    noText.setPosition(260, 125);
                                                    noText.setFillColor(sf::Color::Black);

                                                    while (popup.isOpen()) {
                                                        sf::Event pe;
                                                        while (popup.pollEvent(pe)) {
                                                            if (pe.type == sf::Event::Closed)
                                                                popup.close();
                                                            if (pe.type == sf::Event::MouseButtonPressed) {
                                                                sf::Vector2f mPos(pe.mouseButton.x, pe.mouseButton.y);
                                                                if (yesBtn.getGlobalBounds().contains(mPos)) {
                                                                    general->removeCoins(5);
                                                                    displayMessage = general->getName() + " blocked the coup!";
                                                                    blocked = true;
                                                                    popup.close();
                                                                } else if (noBtn.getGlobalBounds().contains(mPos)) {
                                                                    popup.close();
                                                                }
                                                            }
                                                        }
                                                        popup.clear(sf::Color(240, 240, 250));
                                                        popup.draw(msg);
                                                        popup.draw(yesBtn); popup.draw(noBtn);
                                                        popup.draw(yesText); popup.draw(noText);
                                                        popup.display();
                                                    }

                                                    break; // בדקנו רק גנרל אחד
                                                }
                                            }

                                            if (!blocked) {
                                                target->deactivate();  // ❗ לא לקרוא שוב ל־playTurn!
                                                displayMessage = performer->getName() + " performed a coup on " + target->getName() + ".";
                                            }

                                            game.nextTurn();
                                        }

                                        awaitingTarget = false;
                                        pendingAction = 0;
                                        break;
                                    }



                                    else if (pendingAction == 9) { // Spy Action
                                        if (Spy* spy = dynamic_cast<Spy*>(current)) {
                                            spy->spyOn(*player);
                                            displayMessage = current->getName() + " used spy action on " + player->getName();
                                        }
                                        
                                    }
                                } catch (const std::exception& e) {
                                    displayMessage = "Error: " + std::string(e.what());
                                }
                                awaitingTarget = false;
                                pendingAction = 0;
                                break;
                            }
                        }
                    } else {
                        // Check if clicking on action buttons
                        for (size_t i = 0; i < actionButtons.size(); ++i) {
                            if (actionButtons[i].getGlobalBounds().contains(mousePos)) {
                                std::string action = actionLabels[i];
                                Player* current = game.currentPlayer();
                                
                                try {
                                     if (action == "Gather") {
                                        game.playTurn(1);
                                            displayMessage = game.getLastActionMessage();
                                    }else if (action == "Tax") {
                                        Player* performer = game.currentPlayer();
                                        bool blocked = false;

                                        // חפש Governor במשחק
                                        for (Player* p : game.getPlayers()) {
                                            if (p == performer || !p->isActive()) continue;
                                            if (Governor* gov = dynamic_cast<Governor*>(p)) {
                                                // פתח חלון שואל: האם לחסום?
                                                sf::RenderWindow popup(sf::VideoMode(400, 200), "Block Tax?");
                                                sf::Text msg("Block tax of " + performer->getName() + "?", font, 24);
                                                msg.setPosition(50, 50);
                                                msg.setFillColor(sf::Color(50, 50, 120));

                                                sf::RectangleShape yesBtn(sf::Vector2f(100, 40));
                                                yesBtn.setPosition(70, 120);
                                                yesBtn.setFillColor(sf::Color(180, 240, 200));

                                                sf::RectangleShape noBtn(sf::Vector2f(100, 40));
                                                noBtn.setPosition(230, 120);
                                                noBtn.setFillColor(sf::Color(250, 180, 180));

                                                sf::Text yesText("Yes", font, 20);
                                                yesText.setPosition(100, 125);
                                                yesText.setFillColor(sf::Color::Black);

                                                sf::Text noText("No", font, 20);
                                                noText.setPosition(260, 125);
                                                noText.setFillColor(sf::Color::Black);

                                                while (popup.isOpen()) {
                                                    sf::Event pe;
                                                    while (popup.pollEvent(pe)) {
                                                        if (pe.type == sf::Event::Closed) popup.close();
                                                        if (pe.type == sf::Event::MouseButtonPressed) {
                                                            sf::Vector2f mPos(pe.mouseButton.x, pe.mouseButton.y);
                                                            if (yesBtn.getGlobalBounds().contains(mPos)) {
                                                                game.tryBlockTax(gov);
                                                                displayMessage = gov->getName() + " blocked tax of " + performer->getName();
                                                                blocked = true;
                                                                popup.close();
                                                            } else if (noBtn.getGlobalBounds().contains(mPos)) {
                                                                popup.close();
                                                            }
                                                        }
                                                    }

                                                    popup.clear(sf::Color(240, 240, 250));
                                                    popup.draw(msg);
                                                    popup.draw(yesBtn); popup.draw(noBtn);
                                                    popup.draw(yesText); popup.draw(noText);
                                                    popup.display();
                                                }

                                                break; // יש רק Governor אחד
                                            }
                                        }

                                        if (!blocked) {
                                            game.playTurn(2);  // מבצעים Tax אם לא נחסם
                                            displayMessage = game.getLastActionMessage();
                                        }
                                    }if (action == "Bribe") {
                                        Player* performer = game.currentPlayer();

                                        if (performer->getCoins() < 4) {
                                            displayMessage = performer->getName() + " does not have enough coins to bribe.";
                                            break;
                                        }

                                        performer->removeCoins(4); // ✅ תשלום מראש

                                        bool blocked = false;

                                        for (Player* p : game.getPlayers()) {
                                            if (p == performer || !p->isActive()) continue;
                                            if (Judge* judge = dynamic_cast<Judge*>(p)) {
                                                // Popup...
                                                sf::RenderWindow popup(sf::VideoMode(400, 200), "Block Bribe?");
                                                sf::Text msg("Block bribe of " + performer->getName() + "?", font, 24);
                                                msg.setPosition(50, 50);
                                                msg.setFillColor(sf::Color(50, 50, 120));

                                                sf::RectangleShape yesBtn(sf::Vector2f(100, 40));
                                                yesBtn.setPosition(70, 120);
                                                yesBtn.setFillColor(sf::Color(180, 240, 200));

                                                sf::RectangleShape noBtn(sf::Vector2f(100, 40));
                                                noBtn.setPosition(230, 120);
                                                noBtn.setFillColor(sf::Color(250, 180, 180));

                                                sf::Text yesText("Yes", font, 20);
                                                yesText.setPosition(100, 125);
                                                yesText.setFillColor(sf::Color::Black);

                                                sf::Text noText("No", font, 20);
                                                noText.setPosition(260, 125);
                                                noText.setFillColor(sf::Color::Black);

                                                while (popup.isOpen()) {
                                                    sf::Event pe;
                                                    while (popup.pollEvent(pe)) {
                                                        if (pe.type == sf::Event::Closed) popup.close();
                                                        if (pe.type == sf::Event::MouseButtonPressed) {
                                                            sf::Vector2f mPos(pe.mouseButton.x, pe.mouseButton.y);
                                                            if (yesBtn.getGlobalBounds().contains(mPos)) {
                                                                game.tryBlockBribe(judge);
                                                                displayMessage = judge->getName() + " blocked bribe of " + performer->getName();
                                                                blocked = true;
                                                                popup.close();
                                                            } else if (noBtn.getGlobalBounds().contains(mPos)) {
                                                                popup.close();
                                                            }
                                                        }
                                                    }
                                                    popup.clear(sf::Color(240, 240, 250));
                                                    popup.draw(msg);
                                                    popup.draw(yesBtn); popup.draw(noBtn);
                                                    popup.draw(yesText); popup.draw(noText);
                                                    popup.display();
                                                }

                                                break;
                                            }
                                        }

                                        if (!blocked) {
                                            game.addExtraTurns(1);
                                            game.playTurn(3); // ✅ פעולה מתבצעת – כולל 2 תורות אקסטרה
                                            displayMessage = game.getLastActionMessage();
                                            break;
                                        }
                                    }
                                    else if (action == "Arrest") {
                                        pendingAction = 4;
                                        awaitingTarget = true;
                                        displayMessage = "Choose a target to arrest.";
                                    } else if (action == "Sanction") {
                                        pendingAction = 5;
                                        awaitingTarget = true;
                                        displayMessage = "Choose a target to sanction.";
                                    } else if (action == "Coup") {
                                        pendingAction = 6;
                                        awaitingTarget = true;
                                        displayMessage = "Choose a target to coup.";
                                    } else if (action == "Spy Action") {
                                        pendingAction = 9;
                                        awaitingTarget = true;
                                        displayMessage = "Choose a target to spy on.";
                                    } else if (action == "Invest") {
                                        try {
                                            game.playTurn(8);  // ✅ קורא לפעולה כמו שצריך
                                            displayMessage = game.getLastActionMessage();  // הצגת ההודעה מהמשחק
                                        } catch (const std::exception& e) {
                                            displayMessage = "Error: " + std::string(e.what());
                                        }
                                    }
                                } catch (const std::exception& e) {
                                    displayMessage = "Error: " + std::string(e.what());
                                }
                                break;
                            }
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
                startButton.setPosition(window.getSize().x / 2 - 130, y + 20);
                startText.setPosition(window.getSize().x / 2, y + 20 + 30);
                window.draw(startButton);
                window.draw(startText);
            }
        } else if (gameStarted && !showMainGameScreen) {
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
            startTurnButton.setPosition(window.getSize().x / 2 - 150, y + 40);
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
            
            int indexOnCircle = 0;
            for (size_t j = 0; j < players.size(); ++j) {
                Player* player = players[j];
                if (!player->isActive()) continue;
                
                float angle = 2 * M_PI * indexOnCircle / static_cast<float>(playerCount);
                float x = center.x + radius * std::cos(angle);
                float y = center.y + radius * std::sin(angle);
                indexOnCircle++;
                
                sf::RectangleShape card(sf::Vector2f(150, 60));
                card.setFillColor(j == game.getCurrentTurnIndex() ? sf::Color(180, 255, 180) : sf::Color::White);
                card.setOutlineColor(sf::Color::Black);
                card.setOutlineThickness(2);
                card.setOrigin(75, 30);
                card.setPosition(x, y);
                window.draw(card);
                
                sf::Text nameText(player->getName(), font, 20);
                nameText.setFillColor(sf::Color::Black);
                nameText.setOrigin(nameText.getLocalBounds().width / 2, nameText.getLocalBounds().height / 2);
                nameText.setPosition(x, y - 10);
                window.draw(nameText);
                
                sf::Text roleText(player->getRoleName(), font, 16);
                roleText.setFillColor(sf::Color(100, 100, 160));
                roleText.setOrigin(roleText.getLocalBounds().width / 2, roleText.getLocalBounds().height / 2);
                roleText.setPosition(x, y + 15);
                window.draw(roleText);
                
                sf::Text coinsText("Coins: " + std::to_string(player->getCoins()), font, 16);
                coinsText.setFillColor(sf::Color(120, 90, 20));
                coinsText.setOrigin(coinsText.getLocalBounds().width / 2, coinsText.getLocalBounds().height / 2);
                coinsText.setPosition(x, y + 35);
                window.draw(coinsText);
                
            }
            
            // Draw current player info and action buttons (outside the player loop)
            Player* currentPlayer = game.currentPlayer();
            if (currentPlayer) {
                std::string infoText = "Turn: " + currentPlayer->getName() + " | Coins: " + std::to_string(currentPlayer->getCoins());
                if (awaitingTarget) {
                    infoText += " | " + displayMessage;
                }
                sf::Text info(infoText, font, 22);
                info.setFillColor(sf::Color(40, 40, 40));
                info.setPosition(window.getSize().x - 500, 50);
                window.draw(info);
                
                // Only show action buttons if not awaiting target selection
                if (!awaitingTarget) {
                    std::vector<std::string> actions;
                    if (currentPlayer->getCoins() >= 10) {
                        actions.push_back("Coup");
                    } else {
                        actions = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup"};
                    }
                    
                    std::string role = currentPlayer->getRoleName();
                    if (role == "Spy") actions.push_back("Spy Action");
                    if (role == "Baron") actions.push_back("Invest");

                    
                    actionButtons.clear();
                    actionLabels.clear();
                    
                    float bx = window.getSize().x - 220;
                    float by = 100;
                    
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
                } else {
                    // Show instruction when awaiting target
                    sf::Text instruction("Click on a player to select as target", font, 20);
                    instruction.setFillColor(sf::Color(200, 50, 50));
                    instruction.setPosition(window.getSize().x - 400, 120);
                    window.draw(instruction);
                }
            }
            
            if (!displayMessage.empty()) {
                messageText.setString(displayMessage);
                window.draw(messageText);
            }
        }
        
        window.display();
        try {
    std::string winner = game.getWinner();

    // מנקה את החלון ומציג מסך הסיום
    window.clear(sf::Color(230, 240, 250));  // רקע תואם למשחק

    // כותרת ראשית: GAME OVER
    sf::Text gameOverText("GAME OVER!", font, 64);
    gameOverText.setFillColor(sf::Color(0, 102, 204));  // כחול כמו כותרת המשחק
    gameOverText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::FloatRect titleBounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    gameOverText.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 60);

    // שם המנצח
    sf::Text winnerText("THE WINNER IS: " + winner, font, 40);
    winnerText.setFillColor(sf::Color(0, 150, 130));  // ירוק-כחול עדין – תואם לכפתורי הפעולה
    winnerText.setStyle(sf::Text::Bold);
    sf::FloatRect winnerBounds = winnerText.getLocalBounds();
    winnerText.setOrigin(winnerBounds.width / 2, winnerBounds.height / 2);
    winnerText.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 20);

    // מציג הכל
    window.draw(gameOverText);
    window.draw(winnerText);
    window.display();

    // מחכה לאירוע
    sf::Event event;
    while (window.waitEvent(event)) {
        if (event.type == sf::Event::Closed ||
            event.type == sf::Event::KeyPressed ||
            event.type == sf::Event::MouseButtonPressed) {
            window.close();
            return;
        }
    }

} catch (...) {
    // המשחק עדיין לא נגמר
}


    }
}