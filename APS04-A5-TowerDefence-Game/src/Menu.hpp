#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "config.hpp"
using namespace sf;
using namespace std;

class Menu {
public:
    Menu();

    void setRound(int round);
    void setMoney(int money);
    void setLives(int lives);
    void setBlank(const string& text);

    void setPosition(float x, float y);
    void setImage(string image);
    void draw(RenderWindow* window);
    vector<Sprite*> getImages() const;

    
private:
    Text* roundText;
    Text* moneyText;
    Text* livesText;
    Text* blankText;
    vector<Sprite*> imageSprites;
    Vector2f position;
    float width = MENU_WIDTH;
    float height = MENU_HEIGHT;
};

#endif 
