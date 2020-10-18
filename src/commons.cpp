#include "commons.hpp"


#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <math.h>


int randomI(int min, int max)
{
    if(min == max) return min;
    return rand() % (max - min + 1) + min;
}

int randomIwT(int min, int max, int target, int attempts)
{
    int result = randomI(min, max);

    //std::cout << "(" << min << ", " << max << ") => {" << result;
    for(int i = 1; i < attempts; ++i)
    {
	int attempt = randomI(min, max);
	if(std::abs(result - target) > std::abs(attempt - target)) result = attempt;
	//std::cout << ", " << attempt;
    }
    //std::cout << "|" << target << "} = " << result << std::endl;

    return result;
}

float randomF(float min, float max, float step)
{
    return randomI(std::round(min * 1.f/step), std::round(max * 1.f/step)) * step;
}

float randomFwT(float min, float max, float target, int attempts, float step)
{
    return randomIwT(std::round(min    * 1.f/step),
		     std::round(max    * 1.f/step),
		     std::round(target * 1.f/step), attempts) * step;
}

float radToDeg(float radians)
{
    return radians * 180/M_PI;
}

float normalizeAngle(float angle)
{
    while(angle <       0) angle += 2*M_PI;
    while(angle >= 2*M_PI) angle -= 2*M_PI;
    return angle;
}

float length(sf::Vector2f input)
{
    return sqrt(input.x*input.x + input.y*input.y);
}

float distance(sf::Vector2f a, sf::Vector2f b)
{
    return length(a - b);
}

float determineLightness(sf::Color input)
{
    // https://thoughtbot.com/blog/closer-look-color-lightness
    // values have been normalized (ie divided by 255)
    return ((float)input.r) * 0.00083372549 +
	   ((float)input.g) * 0.00280470588 +
	   ((float)input.b) * 0.00028313725;
}

sf::Color randomColor(std::vector<float> lightRange)
{
    sf::Color result(0, 0, 0);
    do{
	result.r = randomI(0, 255);
	result.g = randomI(0, 255);
	result.b = randomI(0, 255);
    }while(determineLightness(result) < lightRange[0] || determineLightness(result) > lightRange[1]);
    
    return result;
}

float dotProduct(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}

std::string trailingZeroes(float number, int zeroes)
{
    std::ostringstream out;
    out << std::setprecision(zeroes) << std::fixed << number;
    return out.str();
}

