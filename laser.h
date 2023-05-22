#pragma once
#include "settings.h"
class Laser {
public:
	enum LaserType { BLUE, GREEN, RED };
	Laser(LaserType type, sf::Vector2f playerPos);
	void update();
	sf::Sprite& getSprite() { return sprite; }
	sf::FloatRect getHitBox() { return sprite.getGlobalBounds(); }
	float getPosX() { return sprite.getPosition().x; }
	void setHit() { hit = true; }
	bool isHit() { return hit; }
	LaserType getType() { return type; }
private:
	sf::Texture texture;
	sf::Sprite sprite;
	LaserType type;
	bool hit;
};
Laser::Laser(LaserType type, sf::Vector2f playerPos) {
	this->type = type;
	switch (type) {
	case BLUE:
		texture.loadFromFile(BLUE_LASER_FILENAME);
		break;
	case GREEN:
		texture.loadFromFile(GREEN_LASER_FILENAME);
		break;
	case RED:
		texture.loadFromFile(RED_LASER_FILENAME);
		break;
	}
	sprite.setTexture(texture);
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(bounds.width / 2, bounds.height / 2);
	sprite.setPosition(playerPos);
	hit = false;
}
void Laser::update() {
	sprite.move(LASER_SPEED, 0.f);
}