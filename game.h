#pragma once
#include <vector>
#include "settings.h"
#include "player.h"
#include "okean.h"
#include "fish.h"
#include "textObj.h"
#include "bonus.h"
class Game {
public:
	enum GameState { INSTRUCTIONS, PLAY, GAME_OVER, YOU_WIN };
	GameState gameState = PLAY;
	Game() : text("GAME OVER", TEXT_POS, FONT_SIZE1), text1("YOU WIN", TEXT_POS1, FONT_SIZE1) {
		window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
		window.setFramerateLimit(FPS);
		Fish* fish1 = new Fish(Fish::FishType::RED);
		Fish* fish2 = new Fish(Fish::FishType::GREEN);
		Fish* fish3 = new Fish(Fish::FishType::BLUE);
		Fish* fish4 = new Fish(Fish::FishType::RED);
		Fish* fish5 = new Fish(Fish::FishType::GREEN);
		Fish* fish6 = new Fish(Fish::FishType::BLUE);
		fishSprites.push_back(fish1);
		fishSprites.push_back(fish2);
		fishSprites.push_back(fish3);
		fishSprites.push_back(fish4);
		fishSprites.push_back(fish5);
		fishSprites.push_back(fish6);
	}

	void play() {
		while (window.isOpen()) {
			checkEvents();
			update();
			if (player.getHp() <= 0) gameState = GAME_OVER;
			if (player.getPoint() >= 100) gameState = YOU_WIN;
			draw();
		}
	}

private:
	sf::RenderWindow window;
	Player player;
	Okean ocean;
	std::list<Fish*> fishSprites;
	Textobj text;
	Textobj text1;
	std::list<Bonus*> bonusSprites;

	void checkEvents() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	void update() {
		switch (gameState) {
		case PLAY:
			player.update();
			for (auto& fish : fishSprites) {
				fish->update();
			}
			for (auto& bonus : bonusSprites) {
				bonus->update();
			}
			ocean.update();
			checkCollisions();
			break;
		case GAME_OVER:
			text.update("GAME OVER");
			break;
		case YOU_WIN:
			text1.update("YOU WIN");
			break;
		}
	}

	void fishCollideLaser();
	void fishCollideBound();

	void checkCollisions() {
		fishCollideLaser();
		fishCollideBound();
	}

	void draw() {
		switch (gameState) {
		case PLAY:
			window.clear();
			ocean.draw(window);
			player.draw(window);
			for (const auto& fish : fishSprites) {
				fish->draw(window);
			}
			for (const auto& bonus : bonusSprites) {
				bonus->draw(window);
			}
			window.display();
			break;
		case GAME_OVER:
			window.clear();
			ocean.draw(window);
			text.draw(window);
			window.display();
			break;
		case YOU_WIN:
			window.clear();
			ocean.draw(window);
			text1.draw(window);
			window.display();
			break;
		}
	}
};
void Game::fishCollideLaser() {
	auto laserSprites = player.getLaserSprites();
	for (auto& fish : fishSprites) {
		sf::FloatRect fishBounds = fish->getHitBox();
		for (auto& laser : (*laserSprites)) {
			sf::FloatRect laserBounds = laser->getHitBox();
			if (fishBounds.intersects(laserBounds)) {
				if ((fish->getType() == Fish::FishType::BLUE && laser->getType() == Laser::LaserType::BLUE) ||
					(fish->getType() == Fish::FishType::RED && laser->getType() == Laser::LaserType::RED) ||
					(fish->getType() == Fish::FishType::GREEN && laser->getType() == Laser::LaserType::GREEN))
				{
					size_t chance = rand() % 5001;
					if (chance < 1500) {
						size_t bonusType = rand() % Bonus::BonusType::TYPES_QTY;
						Bonus* bonus = new Bonus((Bonus::BonusType)bonusType, fish->getPosition());
						bonusSprites.push_back(bonus);
					}
					fish->spawn();
					laser->setHit();
					player.pointChange();
				}
			}
			if (laser->getPosX() > WINDOW_WIDTH) {
				laser->setHit();
			}
		}

	}
	(*laserSprites).remove_if([](Laser* laser) {return laser->isHit(); });
	for (auto& bonus : bonusSprites) {
		sf::FloatRect playerBounds = player.getHitBox();
		sf::FloatRect bonusBounds = bonus->getHitBox();
		if (bonusBounds.intersects(playerBounds)) {
			bonus->act(player);
			bonus->setDel();
		}
		if (bonus->getPosX() < 0.f - bonusBounds.width) {
			bonus->setDel();
		}
	}
	bonusSprites.remove_if([](Bonus* bonus) {return bonus->isToDel(); });
}

void Game::fishCollideBound() {
	for (auto& fish : fishSprites) {
		sf::FloatRect fishBounds = fish->getHitBox();
		if (fish->getPosition().x <= 1.f - fishBounds.width) {
			player.hpChange();
		}
	}
}