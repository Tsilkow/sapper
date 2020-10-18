#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <tuple>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "resources.hpp"


class TextBox
{
    private:
    sf::Text m_text;
    sf::Vector2f m_position;
    int m_alignment;

    public:
    TextBox(std::shared_ptr<sf::Font>& font, std::string content, sf::Vector2f position, int size, int alignment,
	    sf::Color color=sf::Color(255, 255, 255), int style=0);

    void setContent(std::string content);

    void adjustPosition();

    void draw(sf::RenderTarget& target);

    const sf::Text& getText() {return m_text; }
    const sf::FloatRect getTextBounds() {return m_text.getGlobalBounds(); }
    void printTextBounds() {std::cout << m_text.getGlobalBounds().width << std::endl; }
    void aKiss() {std::cout << m_text.getString().toAnsiString() << std::endl; }
};

class Button
{
    private:
    sf::Text m_text;
    sf::FloatRect m_bounds;

    public:
    Button(sf::Text text, sf::FloatRect bounds);

    bool tick(sf::Vector2f mouse);

    void draw(sf::RenderTarget& target);
};

class InputBox
{
    private:
    sf::Text m_text;
    std::string m_input;
    int m_blinkFreq; // blinking frequency
    int m_lastBlinkStart;
    bool m_blinking;

    public:
    InputBox(sf::Text text, int blinkFreq);

    std::string tick(int ticksPassed, sf::Event& event);

    void draw(sf::RenderTarget& target);

    const sf::Text& getText() {return m_text; }
};

class Interface
{
    private:
    std::string m_name;
    std::shared_ptr<sf::Font> m_font;
    std::map<std::string, TextBox> m_textBoxes;
    std::map<std::string, Button> m_buttons;
    std::map<std::string, InputBox> m_inputBoxes;
    std::vector<sf::Vector2f> m_positions;
    std::vector<int> m_alignments;
    sf::Sprite m_backdrop;

    public:
    Interface(std::string name, std::shared_ptr<sf::Font>& font, sf::Vector2f size,
	      ResourceHolder<sf::Texture, std::string>& textures);

    void addTextBox(std::string key, std::string content, sf::Vector2f position, int size=24,
		    int alignment = 0, sf::Color color = sf::Color(255, 255, 255), int style=0);
    
    bool changeTextBox(std::string key, std::string content);

    bool delTextBox(std::string key);

    bool addButton(std::string key, std::string textKey, sf::FloatRect bounds = sf::FloatRect(0, 0, 0, 0));
    
    bool delButton(std::string key);

    bool addInputBox(std::string key, std::string textKey);
    
    bool delInputBox(std::string key);

    std::pair<std::string, std::string> tick(int ticksPassed, sf::Vector2f mouse, sf::Event& event);

    void draw(sf::RenderTarget& target);
};
