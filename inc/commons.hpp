#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

int randomI(int min, int max);

// random integer with target - n numbers generated within range and then one closest to target is chosen
int randomIwT(int min, int max, int target, int attempts);

float randomF(float min, float max, float step = 0.01);

float randomFwT(float min, float max, float target, int attempts, float step = 0.01);

float radToDeg(float radians);

float normalizeAngle(float angle);

float length(sf::Vector2f input);

float distance(sf::Vector2f a, sf::Vector2f b);

float determineLightness(sf::Color input);
    
sf::Color randomColor(std::vector<float> lightRange);
    
float dotProduct(sf::Vector2f a, sf::Vector2f b);

std::string trailingZeroes(float number, int zeroes = 2);
