#pragma once
#include <SFML/Graphics.hpp>
#include "settings.h"
class Okean {
private:
	sf::Texture texture;
	sf::Sprite sprite;
public:
	Okean();
	void update();
	void draw(sf::RenderWindow& window);
};
Okean::Okean() {
	texture.loadFromFile(OCEAN_FILENAME);
	sprite.setTexture(texture);
	sprite.setPosition(OCEAN_START_POS);
}

void Okean::update() {
	
}

void Okean::draw(sf::RenderWindow& window) {
	window.draw(sprite);
}