#pragma once
#include <vector>
#include <fstream>
#include <string>

using std::vector, std::string, std::ifstream, std::ofstream;

class scoreSystem
{
    public:
    scoreSystem(){ score = 0; }
    // add a high score system

    void incScore(const int i){ score += i; }
    int getScore()const{ return score; }

    private:
    int score;
};