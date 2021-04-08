#pragma once
#include <utility>      // std::pair, std::make_pair
#include <SFML/Graphics.hpp>
#include <iostream>
class Player
{
public:
	Player();
	void setPosition(sf::Vector2f vec);
	void setTexture(sf::Texture& texture);
	sf::Vector2f getPosition();
	sf::RectangleShape getForma();
	float getSpeed();
	void setSpeed(float speed);
	void setLives(int lives);
	void setPoints(int points);
	int getLives();
	int getPoints();
	void reset();
	
private:
	float speed;
	std::pair<float, float> position;
	sf::RectangleShape forma;
	int lives;
	int points;
};

