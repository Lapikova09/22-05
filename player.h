#pragma once
#include <list>
#include "settings.h"
#include "laser.h"
#include "textObj.h"
class Player {
private:
	sf::Sprite sprite;
	sf::Texture texture;
	float speedY;
	void controlPlayer();
	std::list<Laser*> laserSprites;
	sf::Clock timer;
	int curTimeFire, prevTimeFire;
	int hp;
	Textobj hpText;
	int point;
	Textobj pointText;
	bool threeLasersActive;
	bool shieldActive = false;
	int prevTimeShield;
	int curTimeTreeLasersFire, prevTimeTreeLasersFire;

	void createOneLaserBlue();
	void createOneLaserGreen();
	void createOneLaserRed();
	void createAdditionalLasers(Laser laser1);
	void threeLasersUpdate();
public:
	Player();

	void fire();

	void update();

	void draw(sf::RenderWindow& window);

	void hpChange();

	void pointChange();

	sf::FloatRect getHitBox() { return sprite.getGlobalBounds(); }

	std::list<Laser*>* getLaserSprites() { return &laserSprites; }

	int getHp() { return hp; }

	int getPoint() { return point; }

	void activateThreeLasers();

	void deactivateThreeLasers() { threeLasersActive = false; }

	bool isThreeLasersActive() { return threeLasersActive; }

	void activateShield() {
		shieldActive = true;
		prevTimeShield = timer.getElapsedTime().asMilliseconds();
	}

	void deactivateShield() { shieldActive = false; }

	bool shieldIsActive() { return shieldActive; }

	void increaseHp(int heal) { hp = hp + heal; }

	
};

Player::Player() : hpText(std::to_string(hp), sf::Vector2f{ 20.f,0.f }, FONT_SIZE) , pointText(std::to_string(point), sf::Vector2f{ 950.f,0.f }, FONT_SIZE){
	texture.loadFromFile(PLAYER_FILENAME);
	sprite.setTexture(texture);
	sprite.setPosition(START_POS);
	timer.restart();
	hp = 100;
	point = 0;
	prevTimeFire = timer.getElapsedTime().asMilliseconds();
	threeLasersActive = false;
}

void Player::controlPlayer() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		speedY = -SPEED_Y;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		speedY = SPEED_Y;
	}
	if (sprite.getPosition().y < 0) {
		sprite.setPosition(sprite.getPosition().x, 0.f);
	}
	else if (sprite.getPosition().y > WINDOW_HEIGHT - sprite.getGlobalBounds().height) {
		sprite.setPosition(sprite.getPosition().x, WINDOW_HEIGHT - sprite.getGlobalBounds().height);
	}
};
void Player::fire() {
	curTimeFire = timer.getElapsedTime().asMilliseconds();
	if (curTimeFire - prevTimeFire > FIRE_COOLDOWN) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
			createOneLaserBlue();
			prevTimeFire = curTimeFire;
			if (threeLasersActive == true) {
				sf::Vector2f pos = sprite.getPosition();
				sf::FloatRect bounds = sprite.getGlobalBounds();
				sf::Vector2f posUp = sf::Vector2f{ pos.x + bounds.width / 2, pos.y };
				auto laser = new Laser(Laser::LaserType::BLUE, posUp);
				laserSprites.push_back(laser);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
			createOneLaserGreen();
			prevTimeFire = curTimeFire;
			if (threeLasersActive == true) {
				sf::Vector2f pos = sprite.getPosition();
				sf::FloatRect bounds = sprite.getGlobalBounds();
				sf::Vector2f posUp = sf::Vector2f{ pos.x + bounds.width / 2, pos.y };
				auto laser = new Laser(Laser::LaserType::GREEN, posUp);
				laserSprites.push_back(laser);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
			createOneLaserRed();
			prevTimeFire = curTimeFire;
			if (threeLasersActive == true) {
				sf::Vector2f pos = sprite.getPosition();
				sf::FloatRect bounds = sprite.getGlobalBounds();
				sf::Vector2f posUp = sf::Vector2f{ pos.x + bounds.width / 2, pos.y };
				auto laser = new Laser(Laser::LaserType::RED, posUp);
				laserSprites.push_back(laser);
			}
		}
	}
}
void Player::createOneLaserBlue() {
	sf::Vector2f pos = sprite.getPosition();
	sf::FloatRect bounds = sprite.getGlobalBounds();
	sf::Vector2f posCenter = sf::Vector2f{ pos.x + bounds.width / 2,pos.y + bounds.height / 2 };
	auto laser = new Laser(Laser::LaserType::BLUE, posCenter);
	laserSprites.push_back(laser);
}
void Player::createOneLaserRed() {
	sf::Vector2f pos = sprite.getPosition();
	sf::FloatRect bounds = sprite.getGlobalBounds();
	sf::Vector2f posCenter = sf::Vector2f{ pos.x + bounds.width / 2,pos.y + bounds.height / 2 };
	auto laser = new Laser(Laser::LaserType::RED, posCenter);
	laserSprites.push_back(laser);
}
void Player::createOneLaserGreen() {
	sf::Vector2f pos = sprite.getPosition();
	sf::FloatRect bounds = sprite.getGlobalBounds();
	sf::Vector2f posCenter = sf::Vector2f{ pos.x + bounds.width / 2,pos.y + bounds.height / 2 };
	auto laser = new Laser(Laser::LaserType::GREEN, posCenter);
	laserSprites.push_back(laser);
}

void::Player::hpChange() {
	hp = hp - 10;
}

void::Player::pointChange() {
	point = point + 5;
}

void::Player::activateThreeLasers(){
	threeLasersActive = true;
	prevTimeTreeLasersFire = timer.getElapsedTime().asMilliseconds();
}

void Player::threeLasersUpdate() {
	int curTime = timer.getElapsedTime().asMilliseconds();
	if (threeLasersActive) {
		if (curTime - prevTimeTreeLasersFire > FIRE_LASERS_BONUS_COOLDOWN) {
			deactivateThreeLasers();
		}
	}
}

void Player::update() {
	speedY = 0.f;
	controlPlayer();
	sprite.move(0.f, speedY);
	fire();
	for (auto laser : laserSprites) {
		laser->update();
	}
	hpText.update("HP:" + std::to_string(hp));
	pointText.update("POINTS:" + std::to_string(point));
}

void Player::draw(sf::RenderWindow& window) {
	for (auto laser : laserSprites) {
		window.draw(laser->getSprite());
	}
	window.draw(sprite);
	hpText.draw(window);
	pointText.draw(window);
}