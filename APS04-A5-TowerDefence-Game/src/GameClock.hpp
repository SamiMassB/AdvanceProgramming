#ifndef CLOCK_HPP
#define CLOCK_HPP
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace sf;
using namespace std;

class GameClock{
    private:
        Clock* clock;
        int interval; 
    public:
        GameClock(int interv);
        virtual void tick() = 0;
        virtual int Kill() = 0;
        void checkForTick();
        int checkForKill();
};
#endif 
