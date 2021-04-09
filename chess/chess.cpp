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
#include <mysql.h>
#include <windows.h>


int windowWidth = 1000, windowHeight = 800;
int game = 0, nrBeri = 3, fast = 3, ok1 = 0, ok2 = 0, ok3 = 1;
float constantFast = 5;
int highscore = -1;
int ID, ID2, GID;
int punctele_mele = 0, visual_count = 0, puncte_coechipier = 0, bonus = 0;
bool gidSet = false;
std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator
std::uniform_int_distribution<> distr(30, 970); // define the range
std::uniform_int_distribution<> distrp1(30, 450); // define the range
std::uniform_int_distribution<> distrp2(550, 970); // define the range


int delay = 0;

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
		fast = 4;
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
		fast = 4;
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
int qstate;
int main()
{
	//compile with this uncommented when you want to compile for a new version
	ShowWindow(GetConsoleWindow(), SW_HIDE); //SW_RESTORE to bring back

	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	conn = mysql_init(0);

	conn = mysql_real_connect(conn, "sql11.freesqldatabase.com", "sql11403545", "ISkD7nsJW3", "sql11403545", 3306, NULL, 0);

	if (conn) {
		puts("Successful connection to database!");
	}
	else {
		puts("Connection to database has failed!");
		exit(2);
	}

	srand(time(NULL));
	ID = rand() % 15;
	GID = ID;
	std::string query = "insert into game (id) values (" + std::to_string(ID) + ")";
	const char* q = query.c_str();
	qstate = mysql_query(conn, q);
	query = "insert into player (id,game_id1, x, y) values (" + std::to_string(ID) + "," + std::to_string(ID) + ",495.4,44.444)";
	q = query.c_str();
	qstate = mysql_query(conn, q);


	query = "SELECT * FROM player where id=" + std::to_string(ID);
	std::cout << query << std::endl;

	q = query.c_str();
	qstate = mysql_query(conn, q);
	res = mysql_store_result(conn);
	row = mysql_fetch_row(res);
	float x = std::stof(row[2]);
	float y = std::stof(row[3]);
	printf("%f %f\n", x, y);
	x += 0.5;
	y += 0.3;
	query = "update player set x= " + std::to_string(x) + " , y= " + std::to_string(y) + " where id=" + std::to_string(ID) + ";";
	std::cout << query << std::endl;
	q = query.c_str();
	qstate = mysql_query(conn, q);

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
	music.setVolume(20);
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

	sf::Text score1, score2, start, players, versus, room, plus1;
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

	room.setString("Room: " + std::to_string(GID));
	room.setPosition(sf::Vector2f(400, 500));
	room.setFont(font);
	room.setCharacterSize(40);
	room.setFillColor(sf::Color::Red);

	versus.setString("PVP:OFF");
	versus.setPosition(sf::Vector2f(500, 400));
	versus.setFont(font);
	versus.setCharacterSize(40);
	versus.setFillColor(sf::Color::Red);

	plus1.setString("+1");
	plus1.setPosition(sf::Vector2f(400, 500));
	plus1.setFont(font);
	plus1.setCharacterSize(30);
	plus1.setFillColor(sf::Color::Red);

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
			fast += 2;
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
			fast += 2;
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
			if (event.type == event.Closed) {
				window.close();
				query = "delete from player where id=" + std::to_string(ID);
				q = query.c_str();
				mysql_query(conn, q);
			}

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
				if (event.key.code == sf::Keyboard::Num1 && game == 0) { GID++; }
				if (event.key.code == sf::Keyboard::Num2 && game == 0) { GID--; }
			}
		}

		if (game == 1) {
			if (GID != ID && gidSet == false && p2) {
				query = "update player set game_id1= " + std::to_string(GID) + " where id=" + std::to_string(ID) + ";";
				//std::cout << query << std::endl;
				q = query.c_str();
				qstate = mysql_query(conn, q);
				gidSet = true;
			}

			if (ID2 == 0 && p2) {
				std::string query = "SELECT id FROM player WHERE game_id1=" + std::to_string(GID);
				const char* q = query.c_str();
				qstate = mysql_query(conn, q);
				if (!qstate)
				{
					res = mysql_store_result(conn);
					while (row = mysql_fetch_row(res))
					{
						int aux = std::stoi(row[0]);
						if (aux != ID)
							ID2 = aux;
					}
				}
				else
				{
					std::cout << "Query failed: " << mysql_error(conn) << std::endl;
				}
			}
			///partea de miscare
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				if (player1.getPosition().x > 0)
					moveLeft(&player1);
				delay++;
				if (p2 && delay == 2) {
					delay = 0;
					query = "update player set x= " + std::to_string(player1.getPosition().x) + " , y= " + std::to_string(player1.getPosition().y) + " where id=" + std::to_string(ID) + ";";
					//std::cout << query << std::endl;
					q = query.c_str();
					qstate = mysql_query(conn, q);
					//delay = 0;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				if (player1.getPosition().x < 1000)
					moveRight(&player1);
				delay++;
				if (p2 && delay == 2) {
					delay = 0;
					query = "update player set x= " + std::to_string(player1.getPosition().x) + " , y= " + std::to_string(player1.getPosition().y) + " where id=" + std::to_string(ID) + ";";
					//std::cout << query << std::endl;
					q = query.c_str();
					qstate = mysql_query(conn, q);
					//delay = 0;
				}
			}
			if (ID2 && p2) {
				query = "SELECT * FROM player WHERE id=" + std::to_string(ID2);
				q = query.c_str();
				mysql_query(conn, q);
				res = mysql_store_result(conn);
				row = mysql_fetch_row(res);
				if (row == nullptr)
				{
					query = "update player set game_id1= " + std::to_string(ID) + " where id=" + std::to_string(ID) + ";";
					//std::cout << query << std::endl;
					q = query.c_str();
					qstate = mysql_query(conn, q);
					gidSet = false;
					game = 0;
					GID = ID;
					ID2 = 0;
				}
				else
					player2.setPosition(sf::Vector2f(std::stof(row[2]), std::stof(row[3])));
			}
			//player2.setPosition(sf::Vector2f(std::stoi(row[0]), std::stoi(row[1])));

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
			room.setString("Room: " + std::to_string(GID));
			if (p2)
				window.draw(room);

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

							puncte_coechipier++;
							query = "update player set puncte= " + std::to_string(puncte_coechipier) + " where id=" + std::to_string(ID2) + ";";
							q = query.c_str();
							qstate = mysql_query(conn, q);

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
				else if (game == 2 && p2) {
					query = "SELECT * FROM player WHERE id=" + std::to_string(ID2);
					q = query.c_str();
					mysql_query(conn, q);
					res = mysql_store_result(conn);
					row = mysql_fetch_row(res);
					if (row == nullptr)
					{
						query = "update player set game_id1= " + std::to_string(ID) + " where id=" + std::to_string(ID) + ";";
						//std::cout << query << std::endl;
						q = query.c_str();
						qstate = mysql_query(conn, q);
						gidSet = false;
						game = 0;
						GID = ID;
						ID2 = 0;
					}
					else
						player2.setPosition(sf::Vector2f(std::stof(row[2]), std::stof(row[3])));
					window.draw(player2.getForma());
				}
		window.draw(player1.getForma());
		if (p2) {
			window.draw(player2.getForma());
			query = "SELECT puncte FROM player where id=" + std::to_string(ID);
			q = query.c_str();
			qstate = mysql_query(conn, q);
			res = mysql_store_result(conn);
			row = mysql_fetch_row(res);
			if (row != nullptr) {
				int aux = std::stoi(row[0]);
				if (aux > punctele_mele) {
					punctele_mele = aux;

					if (visual_count > 0) {
						bonus++;
						plus1.setString("+" + std::to_string(bonus));
					}
					else
					{
						bonus = 1;
						plus1.setString("+" + std::to_string(bonus));
					}
					visual_count += 40;
					sf::Vector2f a = player1.getPosition();
					a.x -= 60;
					plus1.setPosition(a);

				}
			}
			if (visual_count > 0) {
				visual_count--;
				window.draw(plus1);
			}
			else bonus = 0;
		}

		if (game == 3 || (game == 2 && pvp)) {
			window.draw(line, 2, sf::Lines);
			score2.setString("lives: " + std::to_string(player2.getLives()) + "\npoints: " + std::to_string(player2.getPoints()));
			window.draw(score2);
		}
		score1.setString("lives: " + std::to_string(player1.getLives()) + "\npoints: " + std::to_string(player1.getPoints()) + "\nHIGHSCORE: " + std::to_string(highscore));
		window.draw(score1);
		window.display();
		//printf("%d %d\n", ID, ID2);

	}
}