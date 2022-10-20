#include "Game.h"

void Game::initVariables()
{
	this->window = nullptr;

	//Game logic

	int points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	unsigned int maxEnemies;
	this->points = 0;
	this->enemySpawnTimerMax = 10.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->health = 10;
	this->maxEnemies = 5;
	this->mouseHeld = false;
	this->endgame = false;
}

void Game::initWindow()
{
	this->videoMode.width = 800;
	this->videoMode.height = 600;
	
	this->window = new RenderWindow(this->videoMode, "Game with shapes", Style::Titlebar | Style::Close);

	this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(Vector2f(30.f, 30.f));
	//this->enemy.setScale(Vector2f(2.f,2.f));
	this->enemy.setFillColor(Color::Cyan);
}

void Game::initFonts()
{
	if (!this->font.loadFromFile("Deutsch_Gothic.ttf")) {
		std::cout << "ERROR::GAME::INITFONTS::Failed to load font! " << "\n";
	}
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(Color::White);
	this->uiText.setString("NONE");
}


Game::Game() {
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	this->initFonts();
	this->initText();
}

Game::~Game() {
	delete this->window;
}



void Game::spawnEnemy()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int> (this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	//Randomize
	int type = rand() % 5;

	switch (type) {

	case 0:
		this->enemy.setSize(Vector2f(15.f, 15.f));
		this->enemy.setFillColor(Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(Vector2f(30.f, 30.f));
		this->enemy.setFillColor(Color::Blue);
		break;
	case 2:
		this->enemy.setSize(Vector2f(50.f, 50.f));
		this->enemy.setFillColor(Color::Red);
		break;
	case 3:
		this->enemy.setSize(Vector2f(70.f, 70.f));
		this->enemy.setFillColor(Color::Yellow);
		break;
	case 4:
		this->enemy.setSize(Vector2f(100.f, 100.f));
		this->enemy.setFillColor(Color::Green);
		break;

	default: 
		this->enemy.setSize(Vector2f(100.f, 100.f));
		this->enemy.setFillColor(Color::Cyan);
		break;
	}
	
	this->enemies.push_back(this->enemy);

}

const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndgame() const
{
	return this->endgame;
}

void Game::poolEvents()
{
	while (this->window->pollEvent(this->ev)) {
		switch (this->ev.type){
		case Event::Closed:
			this->window->close();
			break;
		case Event::KeyPressed:
			if (this->ev.key.code == Keyboard::Escape)
				this->window->close();
		
			break;

		
	}
	}
}

void Game::updateMousePosition()
{
	//Updates mouse position
	//Vector2i
	this->mousePosWindow = Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);

}

void Game::updateEnemies()
{
	//Updating timer for spawning enemies
	if (this->enemies.size() < this->maxEnemies) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	//moving and updating enemys
	
	for (int i = 0; i < this->enemies.size();++i) {
		bool deleted = false;
		this->enemies[i].move(0.f, 3.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			
			this->enemies.erase(this->enemies.begin() + i);
			this->health-=1;
			//std::cout << "Health " << this->health << std::endl;
		}
	}

	//check if cliked upon
	if (Mouse::isButtonPressed(Mouse::Left)) {

		if (this->mouseHeld==false) {

			this->mouseHeld = true;
			bool deleted = false;

			for (size_t i = 0; i < this->enemies.size() && deleted == false; ++i) {

				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {

					if (this->enemies[i].getFillColor() == Color::Magenta)
						this->points += 10;
					else if (this->enemies[i].getFillColor() == Color::Blue)
						this->points += 7;
					else if (this->enemies[i].getFillColor() == Color::Red)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == Color::Yellow)
						this->points += 2;
					else if (this->enemies[i].getFillColor() == Color::Green)
						this->points += 1;

					//std::cout << "Points: " << this->points << std::endl;

					//delete the enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
					//gain points
					
				}
			}
		}
		
	}
	else {
		this->mouseHeld = false;
	}
}

void Game::update()
{
	this->poolEvents();

	if (!this->endgame) {

		this->updateMousePosition();

		this->updateText();

		this->updateEnemies();

	}

	//end game condition
	if (this->health <= 0)this->endgame = true;

}

void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << this->points << "\n" << "Health: " << this->health;
	this->uiText.setString(ss.str());
}

void Game::renderText(RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::renderEnemies(RenderTarget& target)
{

	for (auto& w : this->enemies) {
		target.draw(w);
	}

}

void Game::render()
{
	this->window->clear();

	this->renderEnemies(*this->window);

	this->renderText(*this->window);

	this->window->display();
}
