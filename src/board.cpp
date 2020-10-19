#include "board.hpp"


Board::Board(std::shared_ptr<BoardSettings> bSetts, ResourceHolder<sf::Texture, std::string>& textures):
    m_bSetts(bSetts),
    m_tile(textures.get("map").getSize().y),
    m_firstClick(true)
{
    m_square = (int)std::round((float)m_bSetts->m_bounds.width / (float)m_bSetts->m_dimensions.x);

    for(int x = 0; x < m_bSetts->m_dimensions.x; ++x)
    {
	for(int y = 0; y < m_bSetts->m_dimensions.x; ++y)
	{
	    std::vector<sf::Vector2f> position = {
		sf::Vector2f( x   *m_square,  y   *m_square),
		sf::Vector2f((x+1)*m_square,  y   *m_square),
		sf::Vector2f((x+1)*m_square, (y+1)*m_square),
		sf::Vector2f( x   *m_square, (y+1)*m_square)
	    };
	    std::vector<sf::Vector2f> texPos = {
		sf::Vector2f(0     , 0     ),
		sf::Vector2f(m_tile, 0     ),
		sf::Vector2f(m_tile, m_tile),
		sf::Vector2f(0     , m_tile)
	    };

	    for(int i = 0; i < 4; ++i)
	    {
		m_grid.emplace_back(position[i] + 
				    sf::Vector2f(m_bSetts->m_bounds.left, m_bSetts->m_bounds.top),
				    texPos[i]);
	    }
	}
    }

    m_mapTexture = textures.get("map");

    m_states.texture = &m_mapTexture;
}

bool Board::inRange(sf::Vector2i coords)
{
    if(coords.x >= 0 && coords.x < m_data.size() &&
       coords.y >= 0 && coords.y < m_data[0].size())
    {
	return true;
    }
    return false;
}

void Board::generate(sf::Vector2i empty)
{
    m_data     = std::vector< std::vector<int> >(m_bSetts->m_dimensions.x,
						 std::vector<int>(m_bSetts->m_dimensions.y, 0));
    m_revealed = std::vector< std::vector<int> >(m_bSetts->m_dimensions.x,
						 std::vector<int>(m_bSetts->m_dimensions.y, 0));
    
    for(int i = 0; i < m_bSetts->m_minesTotal; ++i)
    {
	sf::Vector2i randomPos;
	do{
	    randomPos = sf::Vector2i(randomI(0, m_bSetts->m_dimensions.x-1),
				     randomI(0, m_bSetts->m_dimensions.y-1));
	}while(randomPos == empty || atCoords(m_data, randomPos) == -1);
	    
	atCoords(m_data, randomPos) = -1;
    }

    for(int x = 0; x < m_bSetts->m_dimensions.x; ++x)
    {
	for(int y = 0; y < m_bSetts->m_dimensions.y; ++y)
	{
	    sf::Vector2i coords(x, y);
	    if(atCoords(m_data, coords) == -1)
	    {
		for(int i = 0; i < m_bSetts->m_neighbourhood.size(); ++i)
		{
		    if(inRange(coords + m_bSetts->m_neighbourhood[i]) &&
		       atCoords(m_data, coords + m_bSetts->m_neighbourhood[i]) != -1)
		    {
			atCoords(m_data, coords + m_bSetts->m_neighbourhood[i]) += 1;
		    }
		}
	    }
	}
    }
}

bool Board::reveal(sf::Vector2i coords)
{
    if(!inRange(coords) || atCoords(m_revealed, coords) == 1) return false;
    atCoords(m_revealed, coords) = 1;
    m_justRevealed.push_back(coords);

    if(atCoords(m_data, coords) == 0)
    {
	for(int i = 0; i < m_bSetts->m_neighbourhood.size(); ++i)
	{
	    reveal(coords + m_bSetts->m_neighbourhood[i]);
	}
    }
    return true;
}

void Board::update()
{
    for(int i = 0; i < m_justRevealed.size(); ++i)
    {
	
	int type = atCoords(m_data, m_justRevealed[i]) + 2; // +2 offset is because texture file is structured so
	std::vector<sf::Vector2f> texPos = {
	    sf::Vector2f( type   *m_tile, 0     ),
	    sf::Vector2f((type+1)*m_tile, 0     ),
	    sf::Vector2f((type+1)*m_tile, m_tile),
	    sf::Vector2f( type   *m_tile, m_tile)
	};

	for(int j = 0; j < 4; ++j)
	{
	    m_grid[(m_justRevealed[i].x * m_data[0].size() + m_justRevealed[i].y)*4 + j].texCoords = texPos[j];
	}
    }
    m_justRevealed.clear();
}

bool Board::stepOn(sf::Vector2i coords)
{
    if(m_firstClick)
    {
	generate(coords);
	m_firstClick = false;
    }
    
    if(atCoords(m_revealed, coords) == 1) return false;
    
    reveal(coords);
    if(atCoords(m_data, coords) == -1)
    {
	std::cout << "GAME OVER" << std::endl;
	return true;
    }
    return false;
}

bool Board::clickAt(sf::Vector2f mouse)
{
    if(m_bSetts->m_bounds.contains(sf::Vector2i(mouse)))
    {
	mouse.x -= m_bSetts->m_bounds.left;
	mouse.y -= m_bSetts->m_bounds.top;

	return stepOn(sf::Vector2i((int)std::floor(mouse.x / m_square), (int)std::floor(mouse.y / m_square)));
    }
    return false;
}

void Board::draw(sf::RenderTarget& target)
{
    update();
    target.draw(&m_grid[0], m_grid.size(), sf::Quads, m_states);
}
