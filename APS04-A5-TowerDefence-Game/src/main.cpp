
#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include "config.hpp"
#include <memory>
#include "system.hpp"

using namespace std;
using namespace sf;

void readData(string file_name, string ground[MAX_ROW]) {
    ifstream iss(file_name);
    int loc = 0;
    while (getline(iss, ground[loc])) {
        loc += 1;
    }
}

RenderWindow createWindow() {
    return RenderWindow(VideoMode(WINDOW_WIDTH + MENU_WIDTH, WINDOW_HEIGHT), "SFML works!");
}

System* initializeSystem(string ground_string[MAX_ROW]) {
    System* system = new System;
    readData("./inp.txt", ground_string);
    system->initializeMenu();
    system->makeGround(ground_string);
    system->addTowerMenu(WINDOW_SIMPLE_MENUICON);
    system->addTowerMenu(WINDOW_ICE_MENUICON);
    system->addTowerMenu(WINDOW_BOMB_MENUICON);
    return system;
}

void processWaveLogic(System* system) {
    for (int i = 0; i < ATTACKING_PLAN.size(); i++) {
        system->startWaveSystem(i);
        system->updateMenuInfo();
    }
}

void handleMouseButtonPress(Event& event, RenderWindow& window, vector<Sprite*>& towerIcons, Sprite& draggingTower, Texture& draggingTexture, bool& isDragging, string& draggingTowerName) {
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        for (int i = 0; i < towerIcons.size(); ++i) {
            if (towerIcons[i]->getGlobalBounds().contains(mousePos)) {
                draggingTexture = *towerIcons[i]->getTexture();
                draggingTower.setTexture(draggingTexture);
                draggingTower.setScale(towerIcons[i]->getScale());
                isDragging = true;
                draggingTowerName = (i == 0) ? SIMPLE_TOWER : (i == 1) ? ICE_TOWER : BOMB_TOWER;
                break;
            }
        }
    }
}

void showTowerPriceHint(System* system, vector<Sprite*>& towerIcons, RenderWindow& window, int& menu_shows_price) {
    Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
    for (int i = 0; i < towerIcons.size(); ++i) {
        if (towerIcons[i]->getGlobalBounds().contains(mousePos)) {
            string price = (i == 0) ? to_string(SIMPLE_TOWER_COST) : (i == 1) ? to_string(ICE_TOWER_COST) : to_string(BOMB_TOWER_COST);
            system->getMenu()->setBlank("Prize: " + price);
            menu_shows_price = 1;
            break;
        }
    }
}

void handleMouseButtonRelease(Event& event, RenderWindow& window, System* system, bool& isDragging, string& draggingTowerName) {
    if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
        if (isDragging) {
            Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
            if (system->canPlaceTowerAt(mousePos.x, mousePos.y)) {
                Vector2f center = system->getCellCenter(mousePos.x, mousePos.y);
                system->addTower(draggingTowerName, center.x, center.y);
            }
            isDragging = false;
        }
    }
}

void drawGame(RenderWindow& window, System* system, bool isDragging, Sprite& draggingTower) {
    window.clear();
    for (auto* tile : system->getGround()) window.draw(*tile);
    for (auto* balloon : system->getBallons()) window.draw(*balloon->getSprite());
    for (auto* bullet : system->getBullets()) window.draw(*bullet->getSprite());
    for (auto* tower : system->getTowers()) {
        window.draw(*tower->getSprite());
        int radius = tower->getRadius();
        Vector2f center = tower->getSprite()->getPosition();
        CircleShape range(radius);
        range.setOrigin(radius, radius);
        range.setPosition(center);
        range.setFillColor(Color(255, 255, 255, 40));
        range.setOutlineColor(Color(255, 255, 255, 80));
        range.setOutlineThickness(1);
        window.draw(range);
    }
    system->drawMenu(&window);

    if (isDragging) {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
        Vector2f centerPos = system->getCellCenter(mousePos.x, mousePos.y);
        float cellRadius = system->getCellSize().first / 2;
        float visualRadius = cellRadius * 1.2f;
        draggingTower.setOrigin(draggingTower.getLocalBounds().width / 2, draggingTower.getLocalBounds().height / 2);
        draggingTower.setPosition(centerPos);
        CircleShape border(visualRadius);
        border.setOrigin(visualRadius, visualRadius);
        border.setPosition(centerPos);
        border.setFillColor(Color(200, 200, 200, 80));
        border.setOutlineColor(system->canPlaceTowerAt(mousePos.x, mousePos.y) ? Color(120, 120, 120, 200) : Color(255, 0, 0, 180));
        border.setOutlineThickness(3);
        window.draw(border);
        window.draw(draggingTower);
    }

    window.display();
}

void runSystem(){
        RenderWindow window = createWindow();
    string ground_string[MAX_ROW];
    System* system = initializeSystem(ground_string);
    vector<Sprite*> towerIcons = system->getTowerMenuSprites();
    processWaveLogic(system);

    Sprite draggingTower;
    Texture draggingTexture;
    bool isDragging = false;
    string draggingTowerName;
    Clock* clock = new Clock;
    int menu_shows_price = 0;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            handleMouseButtonPress(event, window, towerIcons, draggingTower, draggingTexture, isDragging, draggingTowerName);
            if (menu_shows_price) system->getMenu()->setBlank(""), menu_shows_price = 0;
            showTowerPriceHint(system, towerIcons, window, menu_shows_price);
            handleMouseButtonRelease(event, window, system, isDragging, draggingTowerName);
            if (event.type == Event::Closed) window.close();
        }

        if (clock->getElapsedTime().asMilliseconds() > WINDOW_TICK_INTERVAL) {
            system->tick();
            clock->restart();
        }

        drawGame(window, system, isDragging, draggingTower);
    }
}


int main(int argc, char** argv) {
    runSystem();
    return 0;
}
