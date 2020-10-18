#pragma once

#include <iostream>
#include <vector>
#include "json.hpp"


class Scores
{
    private:
    std::vector<std::pair<std::string, int>> m_scores;
    int m_scoresKept;

    public:
    Scores(int scoresKept);

    bool load(std::string filename);

    bool save(std::string filename);

    int potenPlace(int score);
    
    std::vector< std::vector<std::string> > potenList(int newScore = -1);
    
    bool addScore(std::string name, int score);

    const std::pair<std::string, int>& getScore(int place) {return m_scores[place]; }
};
