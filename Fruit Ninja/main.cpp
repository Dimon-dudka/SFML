#include "Game.h"

using namespace sf;

int main()
{
	//rand numbers initialistion
	std::srand(static_cast <unsigned>(time(NULL)));

	Game game;
							//this 2 condition close the app
	while (game.running()&&!game.getEndgame()) {
		
		game.update();



		game.render();

	}

	return 0;
}