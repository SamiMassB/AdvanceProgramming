#include "Menu.hpp"

using namespace sf;
using namespace std;

Menu::Menu() {
    Font* font = new Font;
    if (!font->loadFromFile(FONT_PATH)) {
        cerr << FONT_ERR << endl;
    }
    roundText = new Text;
    moneyText = new Text;
    livesText = new Text;
    blankText = new Text;

    roundText->setFont(*font);
    moneyText->setFont(*font);
    livesText->setFont(*font);
    blankText->setFont(*font);

    roundText->setCharacterSize(20);
    moneyText->setCharacterSize(20);
    livesText->setCharacterSize(20);
    blankText->setCharacterSize(20);

    roundText->setFillColor(Color::Red);
    moneyText->setFillColor(Color::Red);
    livesText->setFillColor(Color::Red);
    blankText->setFillColor(Color::Red);

}

void Menu::setRound(int round) {
    roundText->setString("Round: " + to_string(round));
}

void Menu::setMoney(int money) {
    moneyText->setString("Money: $" + to_string(money));
}

void Menu::setLives(int lives) {
    livesText->setString("Lives: " + to_string(lives));
}

void Menu::setBlank(const string& text) {
    blankText->setString(text);
}

void Menu::setPosition(float x, float y) {
    position = {x, y};
    
    roundText->setPosition(x, y);
    moneyText->setPosition(x, y + 30);
    livesText->setPosition(x, y + 60);
    blankText->setPosition(x, y + 90);

    float imageY = y + 130;
    float imageWidth = width / imageSprites.size() * 1.f;
    for (int i = 0; i < imageSprites.size(); ++i) {
        imageSprites[i]->setPosition(x + i * imageWidth, imageY);
        imageSprites[i]->setScale(imageWidth / imageSprites[i]->getLocalBounds().width, imageWidth / imageSprites[i]->getLocalBounds().width);
    }
}

void Menu::setImage(string path) {
    Texture* texture = new Texture;
    if(!texture->loadFromFile(path))
        cerr << INVALID_PATH << endl;
    imageSprites.push_back(new Sprite);
    imageSprites.back()->setTexture(*texture);
    setPosition(roundText->getPosition().x, roundText->getPosition().y);
}

void Menu::draw(RenderWindow* window) {
    RectangleShape background;
    background.setSize(Vector2f(MENU_WIDTH, MENU_HEIGHT));
    background.setFillColor(Color(30, 30, 60)); 
    background.setPosition(position);
    window->draw(background);
    Color fontColor(255, 255, 255);
    roundText->setFillColor(fontColor);
    moneyText->setFillColor(fontColor);
    livesText->setFillColor(fontColor);
    blankText->setFillColor(fontColor);
    
    window->draw(*roundText);
    window->draw(*moneyText);
    window->draw(*livesText);
    window->draw(*blankText);
    for (const auto* sprite : imageSprites) {
        window->draw(*sprite);
    }


}
vector<Sprite*> Menu::getImages() const {
    return imageSprites;
}
