#pragma once

#include <SFML/OpenGL.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>

#include<iostream>
#include<vector>
#include<ctime>
#include<sstream>

using namespace sf;
/*
Class that acts as the game engine
Wrapper class
*/
class Game
{
private:

	RenderWindow *window;
	VideoMode videoMode;
	Event ev;

	//Mouse Positions
	Vector2i mousePosWindow;
	Vector2f mousePosView;

	//Resources

	Font font;

	//Text

	Text uiText;


	//Game objects
	std::vector<RectangleShape>enemies;

	RectangleShape enemy;

	void initVariables();
	void initWindow();
	void initEnemies();
	void initFonts();
	void initText();

	//Game logic

	int health;
	bool endgame;
	unsigned int points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	unsigned int maxEnemies;
	bool mouseHeld;

public:
	//Constructors/Destructors
	Game();
	virtual ~Game();
	//Functions

	void spawnEnemy();

	const bool running()const;
	const bool getEndgame()const;

	void poolEvents();
	void updateMousePosition();
	void updateEnemies();
	void update();
	void updateText();

	void renderText(RenderTarget &target);
	void renderEnemies(RenderTarget& target);
	void render();
};