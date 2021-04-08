#include "Bere.h"
#include <SFML/Graphics.hpp>

Bere::Bere(float velocity, std::pair<float, float> position) {
	this->velocity = velocity;
	this->position = position;
	this->forma = sf::RectangleShape(sf::Vector2f(20.f, 50.f));
	this->forma.setFillColor(sf::Color::White);
	this->forma.setPosition(this->position.first, this->position.second);

}
Bere::Bere() {
	this->velocity = 0;
	this->position = std::pair<float,float>(200,0);
	this->forma = sf::RectangleShape(sf::Vector2f(60.f, 180.f));
	this->forma.setFillColor(sf::Color::White);
	this->forma.setPosition(this->position.first, this->position.second);
}
void Bere::move() {
	this->position.second += velocity;
	this->forma.setPosition(this->position.first, this->position.second);
	//printf("%f\n", this->position.second);
}

sf::RectangleShape Bere::getForma() {
	return this->forma;
}
std::pair<float,float> Bere::getPosition() {
	return this->position;
}

void Bere::setPosition(std::pair<float, float> position)
{
	this->position = position;
	this->forma.setPosition(this->position.first, this->position.second);

}

void Bere::setVelocity(float velocity) {
	this->velocity = velocity;
}
void Bere::setTexture(sf::Texture &texture) {
	this->forma.setTexture(&texture);
}

