#include <string>
#include "grid.hpp"
#include "Deepl Framework/activationfunctions.hpp"
#include "Deepl Framework/neuronnetwork.hpp"
#include "SFML/Graphics.hpp"

int main() {
    deeplframework::NeuralNetwork mnistModel = deeplframework::NeuralNetwork::ReadBinaryFile("mnistNetwork.bin");
    mnistModel.setActivationForAllLayers(deeplframework::activationFunctions::sigmoid, deeplframework::activationFunctionDerivatives::sigmoid);
    std::vector<double> outputs;
    
    sf::RenderWindow window(sf::VideoMode(1680, 945), "Handwritten Digits Guesser", sf::Style::Close | sf::Style::Titlebar);
    sf::Event event;
    
    sf::RectangleShape background(sf::Vector2f(1680, 945));
    background.setFillColor(sf::Color(255, 255, 255, 255));
    
    sf::Font fffForward;
    fffForward.loadFromFile("FFF_FORWARD.TTF");

    sf::Text description;
    description.setFillColor(sf::Color(19, 19, 19, 255));
    description.setFont(fffForward);
    description.setCharacterSize(40);
    description.setLineSpacing(1.5f);
    description.setString("Draw On The\nGrid To Begin!");
    description.setPosition(1000, 20);

    int fillRadiusSquared = 5;

    sf::Color hoverColor = sf::Color(150, 150, 150, 255);
    sf::Color fillColor = sf::Color(19, 19, 19, 255);

    Grid grid(28, 28, 1, sf::Vector2f(30, 30), 20, 20, sf::Color(230, 230, 230, 255), sf::Color(240, 240, 240, 255));

    while (window.isOpen()) {
        // Check events
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i screenPoint = sf::Mouse::getPosition(window);
                sf::Vector2i box = grid.screenToGridIndex(screenPoint.x, screenPoint.y);

                // No point
                if (box.x == -1) continue;

                for (int c = 0; c < grid.grid.size(); c++) {
                    for (int r = 0; r < grid.grid[c].size(); r++) {
                        double diffX = c - box.x;
                        double diffY = r - box.y;

                        if (diffX * diffX + diffY * diffY <= fillRadiusSquared) {
                            if (event.key.code == sf::Mouse::Left) {
                                grid.grid[c][r].setFillColor(fillColor);
                                grid.setGridPoint(c, r, true);
                            }
                            else if (event.key.code == sf::Mouse::Right) {
                                grid.grid[c][r].setFillColor(((c + r) % 2 == 0) ? grid.color1 : grid.color2);
                                grid.setGridPoint(c, r, false);
                            }
                        }
                    }
                }
                outputs = mnistModel.run(grid.getGridImageVector());
                
                std::vector<double> normalOutput;
                double sum = 0;

                int output = 0;
                int largestOutput = -1;
                for (int i = 0; i < outputs.size(); i++) {
                    if (outputs[i] > largestOutput) {
                        output = i;
                        largestOutput = outputs[i];
                    }
                    sum += outputs[i];
                }

                std::vector<std::string> stringOutputs = {};

                // Calculate softmax with sum
                for (int h = 0; h < outputs.size(); h++) {
                    normalOutput.push_back(outputs[h] / sum);
                    stringOutputs.push_back(std::to_string(h) + ": " + std::to_string((float)((int)(normalOutput[h] * 1000)) / 10).substr(0, 4) + "%\n");
                }

                // Sort list
                for (int m = 0; m < normalOutput.size(); m++) {
                    for (int n = 0; n < normalOutput.size() - 1; n++) {
                        if (normalOutput[n] < normalOutput[n + 1]) {
                            std::swap(normalOutput[n], normalOutput[n + 1]);
                            std::swap(stringOutputs[n], stringOutputs[n + 1]);
                        }
                    }
                }
                
                // Get output string
                std::string text = "";
                for (int l = 0; l < stringOutputs.size(); l++) {
                    text += stringOutputs[l];
                }

                description.setString(text);
            }

            if (event.type == sf::Event::KeyPressed) {
                // Invert board
                if (event.key.code == sf::Keyboard::I) {
                    for (int x = 0; x < grid.cols; x++) {
                        for (int y = 0; y < grid.rows; y++) {
                            if (grid.boolVector[y * 28 + x] == 0) {
                                grid.grid[x][y].setFillColor(fillColor);
                                grid.setGridPoint(x, y, true);
                            }
                            else {
                                grid.grid[x][y].setFillColor(((x + y) % 2 == 0) ? grid.color1 : grid.color2);
                                grid.setGridPoint(x, y, false);
                            }
                        }
                    }
                }
                // Clear board
                if (event.key.code == sf::Keyboard::C) {
                    for (int a = 0; a < grid.cols; a++) {
                        for (int b = 0; b < grid.rows; b++) {
                            grid.grid[a][b].setFillColor(((a + b) % 2 == 0) ? grid.color1 : grid.color2);
                            grid.setGridPoint(a, b, false);
                        }
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        
        grid.drawBG(window, sf::Color(125, 125, 125, 255), sf::Color::Black, 2.0f);
        grid.draw(window);

        window.draw(description);
        window.display();
    }

    return 0;
}

