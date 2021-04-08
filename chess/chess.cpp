#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <deque>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <cmath>
#include "Bere.h"
#include "Player.h"
#include <string>
#include <fstream>
#include <random>

int windowWidth = 1000, windowHeight = 800;
int game = 0, nrBeri = 3, fast = 3, ok1 = 0, ok2 = 0, ok3 = 1;
float constantFast = 5;
int highscore = -1;
std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator
std::uniform_int_distribution<> distr(30, 970); // define the range
std::uniform_int_distribution<> distrp1(30, 450); // define the range
std::uniform_int_distribution<> distrp2(550, 970); // define the range



Bere bere[100], bere2[100];

void initialize(Player* player1, Player* player2, bool pvp) {
	if (pvp) {
		game = 3;
		(*player1).setSpeed(8);
		(*player2).setSpeed(8);
		(*player1).reset();
		(*player2).reset();
		(*player2).setPosition(sf::Vector2f(999, 750));
		nrBeri = 3;
		fast = 1;
		ok1 = 0;
		constantFast = 3;

		for (int i = 0; i < 100; i++) {
			bere[i].setPosition(std::pair<float, float>(distrp1(gen), -100 - rand() % 300));
			bere[i].setVelocity((float)(rand() % fast) + constantFast);
		}
		for (int i = 0; i < 100; i++) {
			bere2[i].setPosition(std::pair<float, float>(distrp2(gen), -100 - rand() % 300));
			bere2[i].setVelocity((float)(rand() % fast) + constantFast);
		}
	}
	else {
		game = 1;
		nrBeri = 3;
		fast = 1;
		ok1 = 0;
		constantFast = 3;
		(*player1).reset();
		(*player1).setSpeed(10);
		(*player2).setSpeed(10);
		for (int i = 0; i < 100; i++) {

			bere[i].setPosition(std::pair<float, float>(distr(gen), -100 - rand() % 300));
			bere[i].setVelocity((float)(rand() % fast) + constantFast);
		}
	}
}

bool checkColision(sf::RectangleShape bere, sf::RectangleShape box) {
	int ok = 0;
	for (int i = 0; i < 60; i++) {
		if (box.getPosition().x - 100 < bere.getPosition().x + i && bere.getPosition().x + i < box.getPosition().x + 100)
		{
			ok++;
			break;
		}
	}

	for (int i = 120; i < 180; i++) {
		if (750 <= bere.getPosition().y + i && bere.getPosition().y + i <= 751)
		{
			ok++;
			break;
		}
	}
	if (ok == 2)
		return true;
	else return false;
}

void moveLeft(Player* player) {
	sf::Vector2f pos = (*player).getForma().getPosition();
	pos.x -= (*player).getSpeed();
	(*player).setPosition(pos);

}

void moveRight(Player* player) {
	sf::Vector2f pos = (*player).getForma().getPosition();
	pos.x += (*player).getSpeed();
	(*player).setPosition(pos);
}

void idle(Player* player, float speed) {
	sf::Vector2f pos = (*player).getForma().getPosition();
	pos.x += speed;
	(*player).setPosition(pos);
}

int main()
{
	//random

	bool p2 = true, pvp = false;

	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "JALEBI BABY", sf::Style::Close);
	window.setFramerateLimit(60);

	//init players
	Player player1, player2;

	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(500, 0)),
		sf::Vertex(sf::Vector2f(500, 1000))
	};



	//player1.setPosition(sf::Vector2f(0, 750));
	sf::IntRect pvpr(500, 400, 80, 40);
	sf::IntRect pr(400, 400, 40, 40);

	//player2 position needs to be reinitialized to start in the right side
	player2.setPosition(sf::Vector2f(950, 750));

	//textures
	sf::Texture denis;
	if (!denis.loadFromFile("images\\denis.png")) { printf("AAAAAAAAA"); }
	sf::Texture cristi;
	if (!cristi.loadFromFile("images\\crsti.png")) { printf("AAAAAAAAA"); }
	sf::Texture cristian;
	if (!cristian.loadFromFile("images\\cristian.png")) { printf("AAAAAAAAA"); }
	sf::Texture bursuc;
	if (!denis.loadFromFile("images\\bursuc.png")) { printf("AAAAAAAAA"); }
	player1.setTexture(denis);
	player2.setTexture(cristian);


	//sounds and music
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("sounds\\woho.wav"))
		return -1;
	sf::Sound sound;
	sound.setBuffer(buffer);
	sf::Music music;
	music.setLoop(true);
	if (!music.openFromFile("sounds\\music.wav"))
		return -1; // error
	music.setVolume(40);
	music.play();

	srand(time(NULL));
	for (int i = 0; i < 100; i++) {
		bere[i].setPosition(std::pair<float, float>(distr(gen), -100 - rand() % 300));
		bere[i].setVelocity((float)(distr(gen) % fast) + constantFast);
		bere[i].setTexture(cristi);

		//bere2[i].setPosition(std::pair<float, float>(distrp1(gen), -100 - rand() % 300));
		//bere2[i].setVelocity((float)(distr(gen) % fast)+ constantFast);
		bere2[i].setTexture(cristi);
	}

	sf::Text score1, score2, start, players, versus;
	sf::Font font;
	if (!font.loadFromFile("FontFile.ttf")) { printf("plm"); }

	//scoreboard in stanga/dreapta
	score1.setFont(font);
	score1.setCharacterSize(20);
	score1.setFillColor(sf::Color::Red);

	score2.setFont(font);
	score2.setCharacterSize(20);
	score2.setFillColor(sf::Color::Cyan);
	score2.setPosition(930, 0);

	//butonul de start si meniu
	start.setString("START");
	start.setPosition(sf::Vector2f(450, 300));
	start.setFont(font);
	start.setCharacterSize(50);
	start.setFillColor(sf::Color::Red);
	sf::RectangleShape startBox(sf::Vector2f(200.f, 100.f));
	startBox.setFillColor(sf::Color::White);
	startBox.setOrigin(sf::Vector2f(200, 100));
	startBox.setPosition(sf::Vector2f(600, 380));

	players.setString("2P");
	players.setPosition(sf::Vector2f(400, 400));
	players.setFont(font);
	players.setCharacterSize(40);
	players.setFillColor(sf::Color::Red);

	versus.setString("PVP:OFF");
	versus.setPosition(sf::Vector2f(500, 400));
	versus.setFont(font);
	versus.setCharacterSize(40);
	versus.setFillColor(sf::Color::Red);

	//citim highscore din fisier si il stocam in highscore
	std::fstream myfile("highscore.txt", std::ios_base::in);
	myfile >> highscore;
	myfile.close();

	while (window.isOpen())
	{
		if (player1.getPoints() == 10 && ok1 == 0) {
			nrBeri++;
			ok1++;
		}
		if (player1.getPoints() == 20 && ok1 == 1) {
			nrBeri++;
			fast++;
			ok1++;
			constantFast += 1;
		}
		if (player1.getPoints() == 30 && ok1 == 2) {
			ok1++;
			constantFast += 1;
			fast++;
		}
		if (player1.getPoints() == 40 && ok1 == 3) {
			fast+=2;
			ok1++;
			constantFast += 1;
			nrBeri++;
		}
		if (player1.getPoints() == 50 && ok1 == 4) {
			fast++;
			ok1++;
		}
		if (player1.getPoints() == 60 && ok1 == 5) {
			nrBeri++;
			fast++;
			ok1++;
			constantFast += 1;

		}
		if (player1.getPoints() == 70 && ok1 == 6) {
			nrBeri++;
			fast+=2;
			ok1++;
		}
		if (player1.getPoints() == 80 && ok1 == 7) {
			fast += 1;
			ok1++;
			constantFast += 1;
		}
		if (player1.getPoints() == 100 && ok1 == 8) {
			fast += 3;
			nrBeri++;
			ok1++;
			constantFast += 1;

		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == event.Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed && game == 0) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					sf::Vector2i pos = sf::Mouse::getPosition(window);

					//rect.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
					if (startBox.getGlobalBounds().contains(window.mapPixelToCoords(pos))) {
						initialize(&player1, &player2, pvp);
					}

					if (pr.contains(pos) && p2 == true) {
						p2 = false;
						players.setString("1P");
					}
					else
						if (pr.contains(pos) && p2 == false)
						{
							p2 = true;
							players.setString("2P");
						}
					if (pvpr.contains(pos) && pvp == true) {
						pvp = false;
						printf("wtff");
						versus.setString("PVP:OFF");
					}
					else
						if (pvpr.contains(pos) && pvp == false)
						{
							pvp = true;
							versus.setString("PVP:ON");
						}

				}

			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::R && game == 2) {
					initialize(&player1, &player2, pvp);
				}
				if (event.key.code == sf::Keyboard::Escape) { game = 0; music.play(); }
				if (event.key.code == sf::Keyboard::P && game != 1) { p2 = false; }
				if (event.key.code == sf::Keyboard::O && game != 1) { p2 = true; }
			}
		}

		if (game == 1) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				if (player2.getPosition().x > 0)
					moveLeft(&player2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				if (player2.getPosition().x < 1000)
					moveRight(&player2);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				if (player1.getPosition().x > 0)
					moveLeft(&player1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				if (player1.getPosition().x < 1000)
					moveRight(&player1);
			}
		}
		if (game == 3) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				if (player2.getPosition().x > 650)
					moveLeft(&player2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				if (player2.getPosition().x < 900)
					moveRight(&player2);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				if (player1.getPosition().x > 50)
					moveLeft(&player1);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				if (player1.getPosition().x < 350)
					moveRight(&player1);
			}
		}

		window.clear();

		if (game == 0) {
			for (int i = 0; i < 10; i++) {

				if (bere[i].getPosition().second < windowHeight)
					bere[i].move();
				else {
					bere[i].setPosition(std::pair<float, float>(distr(gen), -100 - rand() % 500));
					bere[i].setVelocity((float)(rand() % fast) + constantFast);

				}
				if (checkColision(bere[i].getForma(), player1.getForma()))
				{
					//points++;
					bere[i].setPosition(std::pair<float, float>(distr(gen), -100 - rand() % 500));
					bere[i].setVelocity((float)(rand() % fast) + constantFast);

				}
				if (p2) {
					if (checkColision(bere[i].getForma(), player2.getForma()))
					{
						//points++;
						bere[i].setPosition(std::pair<float, float>(distr(gen), -100 - rand() % 500));
						bere[i].setVelocity((float)(rand() % fast) + constantFast);

					}
				}

				window.draw(bere[i].getForma());
				if (player1.getPosition().x < 1000 && ok2 == 0)
				{
					idle(&player1, 0.53);
				}
				if (player1.getPosition().x > 1000)
					ok2++;
				if (player1.getPosition().x > 0 && ok2 == 1)
				{
					idle(&player1, -0.53);
				}
				if (player1.getPosition().x < 0)
					ok2--;
				if (p2) {
					if (player2.getPosition().x <= 1000 && ok3 == 0)
					{
						idle(&player2, 0.33);
					}
					if (player2.getPosition().x > 1000)
						ok3++;
					if (player2.getPosition().x > 0 && ok3 == 1)
					{
						idle(&player2, -0.33);
					}
					if (player2.getPosition().x < 0)
						ok3--;
				}
			}

			window.draw(startBox);
			window.draw(start);
			window.draw(players);
			window.draw(versus);
		}
		else
			if (game == 1) {
				for (int i = 0; i < nrBeri; i++) {

					if (bere[i].getPosition().second < windowHeight)
						bere[i].move();
					else {
						player1.setLives(player1.getLives() - 1);
						sound.play();
						bere[i].setPosition(std::pair<float, float>(distr(gen), -100 - rand() % 500));
						bere[i].setVelocity((float)(rand() % fast) + constantFast);

					}
					if (checkColision(bere[i].getForma(), player1.getForma()))
					{
						player1.setPoints(player1.getPoints() + 1);
						bere[i].setPosition(std::pair<float, float>(distr(gen), -100 - rand() % 500));
						bere[i].setVelocity((float)(rand() % fast) + constantFast);

					}
					if (p2) {
						if (checkColision(bere[i].getForma(), player2.getForma()))
						{
							player1.setPoints(player1.getPoints() + 1);
							bere[i].setPosition(std::pair<float, float>(distr(gen), -100 - rand() % 500));
							bere[i].setVelocity((float)(rand() % fast) + constantFast);

						}
					}

					if (player1.getLives() == 0) {
						game = 2;
						if (player1.getPoints() > highscore)
						{
							highscore = player1.getPoints();
							std::ofstream ofile;
							ofile.open("highscore.txt", std::ios::binary);
							std::string s = std::to_string(highscore);
							char const* pchar = s.c_str();
							ofile.write(pchar, sizeof(pchar));
							ofile.close();
						}
					}
					window.draw(bere[i].getForma());

				}
			}
			else
				if (game == 3) {
					for (int i = 0; i < nrBeri; i++) {

						if (bere[i].getPosition().second < windowHeight)
							bere[i].move();
						else {
							player1.setLives(player1.getLives() - 1);
							sound.play();
							bere[i].setPosition(std::pair<float, float>(distrp1(gen), -100 - rand() % 500));
							bere[i].setVelocity((float)(rand() % fast) + constantFast);

						}
						if (checkColision(bere[i].getForma(), player1.getForma()))
						{
							player1.setPoints(player1.getPoints() + 1);
							bere[i].setPosition(std::pair<float, float>(distrp1(gen), -100 - rand() % 500));
							bere[i].setVelocity((float)(rand() % fast) + constantFast);

						}

						window.draw(bere[i].getForma());

					}
					for (int i = 0; i < nrBeri; i++) {

						if (bere2[i].getPosition().second < windowHeight)
							bere2[i].move();
						else {
							player2.setLives(player2.getLives() - 1);
							sound.play();
							bere2[i].setPosition(std::pair<float, float>(distrp2(gen), -100 - rand() % 500));
							bere2[i].setVelocity((float)(rand() % fast) + constantFast);

						}

						if (checkColision(bere2[i].getForma(), player2.getForma()))
						{
							player2.setPoints(player2.getPoints() + 1);
							bere2[i].setPosition(std::pair<float, float>(distrp2(gen), -100 - rand() % 500));
							bere2[i].setVelocity((float)(rand() % fast) + constantFast);

						}
						window.draw(bere2[i].getForma());


					}

					if (player1.getLives() == 0 || player2.getLives() == 0) {
						game = 2;
					}
				}
		window.draw(player1.getForma());
		if (p2) {
			window.draw(player2.getForma());
		}

		if (game == 3 || (game == 2 && pvp)) {
			window.draw(line, 2, sf::Lines);
			score2.setString("lives: " + std::to_string(player2.getLives()) + "\npoints: " + std::to_string(player2.getPoints()));
			window.draw(score2);
		}
		score1.setString("lives: " + std::to_string(player1.getLives()) + "\npoints: " + std::to_string(player1.getPoints()) + "\nHIGHSCORE: " + std::to_string(highscore));
		window.draw(score1);
		window.display();
		//printf("%d\n", points);

	}
}