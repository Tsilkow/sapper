#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "commons.hpp"
#include "resources.hpp"

struct BoardSettings
{
    sf::IntRect m_bounds;
    sf::Vector2i m_dimensions;
    int m_minesTotal;
    std::vector<sf::Vector2i> m_neighbourhood;
};

template <typename T>
T& atCoords(std::vector< std::vector<T> >& data, sf::Vector2i coords)
{
    return data[coords.x][coords.y];
}

class Board
{
    private:
    std::shared_ptr<BoardSettings> m_bSetts;
    float m_square; // width and height of one square in the grid
    int m_tile; // width and height of one square's texture
    std::vector< std::vector<int> > m_data; // -1 is mine, 0 is no neighbours, 1 is one neighbour and so on ...
    std::vector< std::vector<int> > m_revealed; // 1 if revealed - bool was not used, because of vector<bool> template specialization
    std::vector<sf::Vertex> m_grid; // graphical representation
    std::vector<sf::Vertex> m_interface; // interface elements independent from data
    std::vector<sf::Vector2i> m_justRevealed;
    sf::RenderStates m_states;
    bool m_firstClick;
    sf::Texture m_mapTexture;

    bool inRange(sf::Vector2i coords);

    void generate(sf::Vector2i empty);

    bool reveal(sf::Vector2i coords);

    void update();

    public:
    Board(std::shared_ptr<BoardSettings>, ResourceHolder<sf::Texture, std::string>& textures);

    // true means a mine went off
    bool stepOn(sf::Vector2i coords);

    bool clickAt(sf::Vector2f mouse);
    
    void draw(sf::RenderTarget& target);
};
