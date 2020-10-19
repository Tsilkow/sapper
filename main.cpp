// Tsilkow

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"
#include "interface.hpp"
#include "scores.hpp"
#include "resources.hpp"
#include "board.hpp"


using namespace std;

int main()
{
    srand(time(NULL));

    BoardSettings bSetts =
    {
	sf::IntRect(-400, -400, 800, 800), // m_bounds
	sf::Vector2i(10, 10),              // m_dimensions
        10,                                // m_minesTotal
	{                                  // m_neighbourhood
	    sf::Vector2i( 0, -1),
	    sf::Vector2i(-1, -1),
	    sf::Vector2i(-1,  0),
	    sf::Vector2i(-1,  1),
	    sf::Vector2i( 0,  1),
	    sf::Vector2i( 1,  1),
	    sf::Vector2i( 1,  0),
	    sf::Vector2i( 1, -1)
	}
    };

    shared_ptr<BoardSettings> shr_bSetts = make_shared<BoardSettings>(bSetts);

    string scoreFilename = "scores.json";
    Scores scores(10);
    if(!scores.load(scoreFilename))
    {
	cout << "Error! Could not read score file. Generating an empty one" << endl;
    }

    std::string fontFilename = "digital-7 (mono).ttf";
    sf::Font font;
    if(!font.loadFromFile(fontFilename))
    {
	cout << "!ERROR! Font could not be loaded! Aborting ..." << endl;
	return 1;
    }
    shared_ptr<sf::Font> shr_font = make_shared<sf::Font>(font);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Clear Asteroids");
    window.setFramerateLimit(60);

    ResourceHolder<sf::Texture, std::string> textures;
    textures.load("map", "data/map.png");
    textures.load("Menu::backdrop", "data/Menu::backdrop.png");
    textures.load("Play::backdrop", "data/Play::backdrop.png");
    textures.load("Scores::backdrop", "data/Scores::backdrop.png");
    
    Board board(shr_bSetts, textures);
    Interface menuInterface("Menu", shr_font, sf::Vector2f(800, 800), textures);
    Interface playInterface("Play", shr_font, sf::Vector2f(800, 800), textures);
    Interface scoresInterface("Scores", shr_font, sf::Vector2f(800, 800), textures);
    
    menuInterface.addTextBox("title", "Sapper", sf::Vector2f(0, -300), 60, 0);
    menuInterface.addTextBox("tPlay", "Play"  , sf::Vector2f(0, -50), 60, 0);
    menuInterface.addButton("bPlay", "tPlay", sf::FloatRect(0, 0, 0, 0));
    menuInterface.addTextBox("tScores", "Scores", sf::Vector2f(0,  50), 60, 0);
    menuInterface.addButton("bScores", "tScores", sf::FloatRect(0, 0, 0, 0));
    menuInterface.addTextBox("tExit", "Exit", sf::Vector2f(0,  150), 60, 0);
    menuInterface.addButton("bExit", "tExit", sf::FloatRect(0, 0, 0, 0));
    menuInterface.addTextBox("LastScore", "Last score = 0", sf::Vector2f(-400,  365), 30, -1);

    playInterface.addTextBox("timer", "0.00", sf::Vector2f(-390, -400), 50, -1);
    playInterface.addTextBox("minesLeft", "0", sf::Vector2f(325, -400), 50, -1); 
    
    scoresInterface.addTextBox("title", "Highscores", sf::Vector2f(0, -300), 60, 0);
    for(int i = 0; i < 10; ++i)
    {
	string numeral = std::to_string(i+1) + ". ";
	if(i != 9) numeral = " " + numeral;
	scoresInterface.addTextBox(std::to_string(i+1)          , numeral,
				   sf::Vector2f(-250, -225 + 50*i), 50, -1);
	scoresInterface.addTextBox(std::to_string(i+1) + "name" , "", 
				   sf::Vector2f(-150, -225 + 50*i), 50, -1);
	scoresInterface.addTextBox(std::to_string(i+1) + "score",  "", 
				   sf::Vector2f(200, -225 + 50*i), 50, -1);
    }
    scoresInterface.addTextBox("tBack", "Back", sf::Vector2f(0, 300), 60, 0);
    scoresInterface.addButton("bBack", "tBack", sf::FloatRect(0, 0, 0, 0));

    sf::View actionView(sf::Vector2f(0.f, 0.f), sf::Vector2f(800, 800));
    window.setView(actionView);

    enum GameState{Menu, Play, Scores};
    GameState currState = GameState::Play;
    bool hasFocus = true;
    bool newScore = false;
    bool firstTimeInScores = true;
    int ticksPassed = 0;
    int score = 0;
    int lastScore = 0;

    while(window.isOpen())
    {
	sf::Event event;
	std::pair<std::string, std::string> input;
	
	window.clear();
	
	while (window.pollEvent(event))
	{
	    switch(event.type)
	    {
		case sf::Event::Closed:
		    window.close();
		    break;
		case sf::Event::LostFocus:
		    hasFocus = false;
		    //std::cout << "LOST FOCUS" << std::endl;
		    break;
		case sf::Event::GainedFocus:
		    hasFocus = true;
		    //std::cout << "GAINED FOCUS" << std::endl;
		    break;
		case sf::Event::KeyPressed:
		    if(hasFocus)
		    {
			switch(event.key.code)
			{
			    case sf::Keyboard::Escape:
				window.close();
				break;
			}
		    }
		    break;
	    }

	    if(hasFocus)
	    {
		switch(currState)
		{
		    case GameState::Menu:
			break;

		    case GameState::Play:
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
			    board.clickAt(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
			}
			break;

		    case GameState::Scores:
			break;
		}
	    }
	}

	switch(currState)
	{
	    case GameState::Menu:
		menuInterface.draw(window);
		break;

	
	    case GameState::Play:

		

	        playInterface.draw(window);
		board.draw(window);
		
		if(hasFocus)
		{
		    ++ticksPassed;
		}
		break;
		
	    case GameState::Scores:
		
	        scoresInterface.draw(window);
		break;
	
	}
	window.display();
    }
    return 0;
}
