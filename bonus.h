#pragma once
#include "settings.h"
#include "player.h"
class Bonus {
public:
	enum BonusType { MULTY_LASER, HP, SHIELD, TYPES_QTY };
	Bonus(BonusType type, sf::Vector2f position);
	void update();
	void draw(sf::RenderWindow& window);
	sf::FloatRect getHitBox();
	sf::Vector2f getPosition();
	void setDel();
	bool isToDel();
	void act(Player& player);
	BonusType getType();
	float getPosX();
private:
	sf::Texture texture;
	sf::Sprite sprite;
	BonusType type;
	bool del = false;
};

Bonus::Bonus(BonusType type, sf::Vector2f position) {
	this->type = type;
	switch (type) {
	case MULTY_LASER:
		texture.loadFromFile(MULTY_BONUS_FILENAME);
		break;
	case HP:
		texture.loadFromFile(HP_BONUS_FILENAME);
		break;
	case SHIELD:
		texture.loadFromFile(SHIELD_BONUS_FILENAME);
		break;
	}
	sprite.setTexture(texture);
	sprite.setPosition(position);
}
void Bonus::update() { sprite.move(BONUS_SPEED, 0.f); }
void Bonus::draw(sf::RenderWindow& window) { window.draw(sprite); }
sf::FloatRect Bonus::getHitBox() { return sprite.getGlobalBounds(); }
sf::Vector2f Bonus::getPosition() { return sprite.getPosition(); }
bool Bonus::isToDel() { return del; }
void Bonus::setDel() { del = true; }
Bonus::BonusType Bonus::getType() { return type; }
float Bonus::getPosX() { return sprite.getPosition().x; }
void Bonus::act(Player& player) {
	switch (type) {
	case MULTY_LASER:
		player.activateThreeLasers();
		break;
	case HP:
		player.increaseHp(HEAL);
		break;
	case SHIELD:
		player.activateShield();
		break;
	}
}
