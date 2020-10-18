#include "interface.hpp"

#include <math.h>


TextBox::TextBox(std::shared_ptr<sf::Font>& font, std::string content, sf::Vector2f position, int size,
		 int alignment, sf::Color color, int style):
    m_text(content, *font, size),
    m_position(position),
    m_alignment(alignment)
{
    m_text.setFillColor(color);
    switch(style)
    {
	case 0: m_text.setStyle(sf::Text::Style::Regular); break;
	case 1: m_text.setStyle(sf::Text::Style::Bold); break;
	case 2: m_text.setStyle(sf::Text::Style::Italic); break;
	case 3: m_text.setStyle(sf::Text::Style::Underlined); break;
	case 4: m_text.setStyle(sf::Text::Style::StrikeThrough); break;
    }

    adjustPosition();
}

void TextBox::setContent(std::string content)
{
    m_text.setString(content);

    adjustPosition();
}

void TextBox::adjustPosition()
{
    float width = m_text.getGlobalBounds().width;

    m_text.setPosition(
	   sf::Vector2f(
		  std::round(m_position.x - (m_alignment+1) * width * 0.5f),
		  m_position.y));
}

void TextBox::draw(sf::RenderTarget& target)
{
    target.draw(m_text);
}

Button::Button(sf::Text text, sf::FloatRect bounds):
    m_text(text), m_bounds(bounds)
{
    m_text.setFillColor(sf::Color::White);
    m_text.setOutlineColor(sf::Color::White);
    m_text.setOutlineThickness(0);
}

bool Button::tick(sf::Vector2f mouse)
{
    if(m_bounds.contains(mouse))
    {
	m_text.setFillColor(sf::Color::Black);
	m_text.setOutlineThickness(3);
	return true;
    }
    m_text.setFillColor(sf::Color::White);
    m_text.setOutlineThickness(0);
    return false;
}

void Button::draw(sf::RenderTarget& target)
{
    target.draw(m_text);
}

InputBox::InputBox(sf::Text text, int blinkFreq):
    m_text(text), m_lastBlinkStart(0), m_blinkFreq(blinkFreq)
{
    m_text.setString("");
}

std::string InputBox::tick(int ticksPassed, sf::Event& event)
{
    if(event.type == sf::Event::TextEntered)
    {
	if(event.text.unicode == '\b' && m_input.size() > 0) m_input.erase(m_input.size() - 1, 1);
	else if(event.text.unicode >= 32 && event.text.unicode < 128 && m_input.size() <= 13)
	{
	    m_input += static_cast<char>(event.text.unicode);
	}
    }

    if     (ticksPassed - m_lastBlinkStart <   m_blinkFreq) m_blinking = true;
    else if(ticksPassed - m_lastBlinkStart < 2*m_blinkFreq) m_blinking = false;
    else
    {
	m_lastBlinkStart = ticksPassed;
	m_blinking = true;
    }

    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) return m_input;
    return "";
}

void InputBox::draw(sf::RenderTarget& target)
{
    if(m_blinking) m_text.setString(m_input + "_");
    else m_text.setString(m_input);
    target.draw(m_text);
    m_text.setString(m_input);
}

Interface::Interface(std::string name, std::shared_ptr<sf::Font>& font, sf::Vector2f size,
		     ResourceHolder<sf::Texture, std::string>& textures):
    m_name(name),
    m_font(font)
{
    m_backdrop.setPosition(-size * 0.5f);
    //m_backdrop.setSize(size);
    m_backdrop.setTexture(textures.get(name + "::backdrop"));
}

void Interface::addTextBox(std::string key, std::string content, sf::Vector2f position, int size,
			   int alignment, sf::Color color, int style)
{
    TextBox temp(m_font, content, position, size, alignment, color, style);
    
    if(m_textBoxes.find(key) == m_textBoxes.end()) m_textBoxes.insert(make_pair(key, temp));
    else
    {
	std::cout << "TextBox " << key << "already exist" << std::endl;
	m_textBoxes.find(key)->second = temp;
    }
}

bool Interface::changeTextBox(std::string key, std::string content)
{
    auto found = m_textBoxes.find(key);
    if(found != m_textBoxes.end())
    {
        found->second.setContent(content);
	return true;
    }
    else
    {
	std::cout << "Error! Could not find TextBox with key " << key << "." << std::endl;
	return false;
    }
}

bool Interface::delTextBox(std::string key)
{
    auto found = m_textBoxes.find(key);
    if(found != m_textBoxes.end())
    {
	m_textBoxes.erase(found);

	return true;
    }
    return false;
}

bool Interface::addButton(std::string key, std::string textKey, sf::FloatRect bounds)
{
    auto found = m_textBoxes.find(textKey);
    if(found != m_textBoxes.end())
    {
	if(bounds.width == 0 || bounds.height == 0) bounds = found->second.getText().getGlobalBounds();
	
	m_buttons.insert({key, Button(found->second.getText(), bounds)});
	m_textBoxes.erase(found);
	
	return true;
    }
    return false;
}

bool Interface::delButton(std::string key)
{
    auto found = m_buttons.find(key);
    if(found != m_buttons.end())
    {
	m_buttons.erase(found);

	return true;
    }
    return false;
}

bool Interface::addInputBox(std::string key, std::string textKey)
{
    auto found = m_textBoxes.find(textKey);
    if(found != m_textBoxes.end())
    {
	m_inputBoxes.insert({key, InputBox(found->second.getText(), 60)});
	//m_textBoxes.erase(found);
	
	return true;
    }
    return false;
}

bool Interface::delInputBox(std::string key)
{
    auto found = m_inputBoxes.find(key);
    if(found != m_inputBoxes.end())
    {
	m_inputBoxes.erase(found);

	return true;
    }
    return false;
}
		     
std::pair<std::string, std::string> Interface::tick(int ticksPassed, sf::Vector2f mouse, sf::Event& event)
{
    std::pair<std::string, std::string> result;
    std::vector< std::map<std::string, InputBox>::iterator > toRemove;
    
    for(auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
    {
	if(it->second.tick(mouse)) result.first = it->first;
    }
    
    for(auto it = m_inputBoxes.begin(); it != m_inputBoxes.end(); ++it)
    {
	result.second = it->second.tick(ticksPassed, event);
	if(result.second != "") toRemove.emplace_back(it);
    }

    for(auto it = toRemove.begin(); it != toRemove.end(); ++it)
    {
	m_inputBoxes.erase(*it);
    }
    
    return result;
}

void Interface::draw(sf::RenderTarget& target)
{
    target.draw(m_backdrop);
    
    for(auto it = m_textBoxes.begin(); it != m_textBoxes.end(); ++it)
    {
        it->second.draw(target);
    }
    
    for(auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
    {
        it->second.draw(target);
    }
    
    for(auto it = m_inputBoxes.begin(); it != m_inputBoxes.end(); ++it)
    {
        it->second.draw(target);
    }
}
