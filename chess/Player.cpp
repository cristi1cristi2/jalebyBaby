#include "Player.h"

Player::Player() {
	this->speed = 8;
	this->forma = sf::RectangleShape(sf::Vector2f(300, 200));
	this->forma.setOrigin(sf::Vector2f(150.f, 100.f));
	this->forma.setFillColor(sf::Color::White);
	this->forma.setPosition(sf::Vector2f(0, 750));
	this->points = 0;
	this->lives = 3;
}
void Player::setPosition(sf::Vector2f vec) {
	this->forma.setPosition(vec);
}
void Player::setTexture(sf::Texture& texture) {
	this->forma.setTexture(&texture);
}
sf::Vector2f Player::getPosition() {
	return this->forma.getPosition();
}
sf::RectangleShape Player::getForma() {
	return this->forma;
}
float Player::getSpeed(){
	return this->speed;
}
void Player::setSpeed(float speed) {
	this->speed = speed;
}
void Player::setLives(int lives) {
	this->lives = lives;
}
void Player::setPoints(int points) {
	this->points = points;
}
int Player::getLives() {
	return this->lives;
}
int Player::getPoints() {
	return this->points;
}
void Player::reset() {
	this->lives = 3;
	this->points = 0;
	this->forma.setPosition(0, 750);
}