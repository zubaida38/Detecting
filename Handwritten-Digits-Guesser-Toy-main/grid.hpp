#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Grid {
	int rows, cols;
	float xOffset, yOffset;
	float spaceBetweenBoxes;

	sf::Color color1, color2;
	sf::Vector2f boxDimensions;
	std::vector<std::vector<sf::RectangleShape>> grid;
	std::vector<bool> boolVector;

	Grid(int _cols, int _rows, float _spaceBetweenBoxes, sf::Vector2f _boxDimensions, float _xOffset, float _yOffset, sf::Color _color1, sf::Color _color2) {
		rows = _rows;
		cols = _cols;
		boxDimensions = _boxDimensions;
		spaceBetweenBoxes = _spaceBetweenBoxes;

		xOffset = _xOffset;
		yOffset = _yOffset;

		color1 = _color1;
		color2 = _color2;

		// Instantiate grid
		for (int c = 0; c < cols; c++) {
			grid.push_back({});

			for (int r = 0; r < rows; r++) {
				sf::RectangleShape box(_boxDimensions);
				box.setPosition(sf::Vector2f(c * (_boxDimensions.x + spaceBetweenBoxes) + xOffset, r * (_boxDimensions.y + spaceBetweenBoxes) + yOffset));
				if ((c + r) % 2 == 0)
					box.setFillColor(_color1);
				else
					box.setFillColor(_color2);

				grid[c].push_back(box);
				boolVector.push_back(false);
			}
		}
	}

	// Returns [-1, -1] if no point on the grid overlaps with the screen point
	sf::Vector2i screenToGridIndex(int x, int y) {
		x -= xOffset;
		y -= yOffset;

		for (int c = 0; c < cols; c++) {
			for (int r = 0; r < rows; r++) {
				float lowerBoundX = c * (boxDimensions.x + spaceBetweenBoxes);
				float lowerBoundY = r * (boxDimensions.y + spaceBetweenBoxes);
				float upperBoundX = lowerBoundX + boxDimensions.x;
				float upperBoundY = lowerBoundY + boxDimensions.y;

				if (x >= lowerBoundX && x <= upperBoundX && y >= lowerBoundY && y <= upperBoundY) {
					return sf::Vector2i(c, r);
				}
			}
		}
		return sf::Vector2i(-1, -1);
	}

	void drawBG(sf::RenderWindow& window, sf::Color bgColor, sf::Color outlineColor = sf::Color::Black, float outlineWidth = 1.0f) {
		sf::RectangleShape bg(sf::Vector2f(cols * boxDimensions.x + (cols - 1) * spaceBetweenBoxes, rows * boxDimensions.y + (rows - 1) * spaceBetweenBoxes));
		bg.setPosition(xOffset, yOffset);
		bg.setFillColor(bgColor);
		bg.setOutlineColor(outlineColor);
		bg.setOutlineThickness(outlineWidth);

		window.draw(bg);
	}

	void draw(sf::RenderWindow& window) {
		for (int c = 0; c < cols; c++) {
			for (int r = 0; r < rows; r++) {
				window.draw(grid[c][r]);
			}
		}
	}

	void setGridPoint(int i, int j, bool state) {
		boolVector[j * cols + i] = state;
	}

	std::vector<double> getGridImageVector() {
		std::vector<double> values;

		for (int i = 0; i < boolVector.size(); i++) {
			values.push_back((boolVector[i]) ? 1.0 : 0.0);
		}

		return values;
	}
};
