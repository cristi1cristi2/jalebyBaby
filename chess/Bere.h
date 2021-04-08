#pragma once
#include <utility>      // std::pair, std::make_pair
#include <SFML/Graphics.hpp>
class Bere
{
public:
	Bere();
	Bere(float velocity, std::pair<float,float> position);
	void move();
	sf::RectangleShape getForma();
	std::pair<float, float> getPosition();
	void setPosition(std::pair<float, float> position);
	void setVelocity(float velocity);
	void setTexture(sf::Texture &texture);

private:
	float velocity;
	std::pair <float, float> position;
	sf::RectangleShape forma;
};

