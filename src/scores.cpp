#include "scores.hpp"

#include <iomanip>
#include <fstream>


using namespace nlohmann;

Scores::Scores(int scoresKept):
    m_scoresKept(scoresKept)
{
    ;
}

bool Scores::load(std::string filename)
{
    std::fstream file(filename, std::fstream::in);

    if(!file.good()) return false;

    m_scores.clear();

    json parse = json::parse(file);

    try{
	for(int i = 0; i < parse["scores"].size() && i < m_scoresKept; ++i)
	{
	    m_scores.emplace_back(std::pair<std::string, int>(parse["scores"][i][0].get<std::string>(),
							      parse["scores"][i][1].get<int>()));
	}
    }
    catch(json::exception& e)
    {
	std::cout << "Score file error = " << e.what() << std::endl;
    }
    
    file.close();

    return true;
}

bool Scores::save(std::string filename)
{
    std::fstream file(filename, std::fstream::out);

    if(!file.good()) return false;

    json parse;

    parse["scores"] = {};

    for(int i = 0; i < m_scores.size(); ++i)
    {
	parse["scores"][i][0] = m_scores[i].first;
	parse["scores"][i][1] = m_scores[i].second;
    }
    
    //std::cout << std::setw(4) << parse << std::endl;

    file << parse;
    file.close();

    return true;
}

int Scores::potenPlace(int score)
{
    int result = -1;
    bool scoreFound = false;

    for(int i = 0; i < m_scores.size(); ++i)
    {
	if(score > m_scores[i].second)
	{
	    result = i;
	    scoreFound = true;
	}
	if(scoreFound) break;
    }
    if(!scoreFound && m_scores.size() < m_scoresKept) result = m_scores.size();
    
    return result;
}

std::vector< std::vector<std::string> > Scores::potenList(int newScore)
{
    std::vector< std::vector<std::string> > result (2, std::vector<std::string>(m_scoresKept, ""));
    int newPlacement = potenPlace(newScore);

    for(int i = 0; i < m_scores.size(); ++i)
    {
	result[0][i] = m_scores[i].first;
	result[1][i] = std::to_string(m_scores[i].second);
    }

    if(newScore != -1 && newPlacement >= 0)
    {
	std::cout << "heyo, time for some empty line!" << newPlacement << std::endl;
	result[0].insert(result[0].begin() + newPlacement, "");
	result[1].insert(result[1].begin() + newPlacement, std::to_string(newScore));
        result[0].pop_back();
        result[1].pop_back();
    }

    //for(int i = 0; i < m_scoresKept; ++i)
    //{
    //std::cout << result[0][i] << " " << result[1][i] << std::endl;
    //}

    return result;
}

bool Scores::addScore(std::string name, int score)
{
    m_scores.insert(m_scores.begin() + potenPlace(score), std::pair<std::string, int>(name, score));
    
    while(m_scores.size() > m_scoresKept) m_scores.pop_back();

    return true;
}
