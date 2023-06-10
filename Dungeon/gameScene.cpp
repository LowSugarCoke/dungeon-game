#include "gameScene.h"

#include <QRandomGenerator>
#include <QTimer>

#include "brickItem.h"
#include "hero.h"
#include "monster.h"
#include "level.h"
#include "resource.h"

GameScene::GameScene(QObject* parent)
    : QGraphicsScene(parent) {

}

void GameScene::setSceneImg(const QString& kSceneImg) {
    QImage backgroundImage(kSceneImg);
    setBackgroundBrush(QBrush(backgroundImage));
}

void GameScene::generatorRandomMap(const QString& kBrickImg, Difficulty difficulty) {
    MazeGenerator maze(20, 10, difficulty);  // Create a 30x15 maze
    QImage brickImage(kBrickImg);

    // Adjust the size of the pixmap to fit your screen resolution
    int brickSize = 40;  // Adjust this to get your desired brick size
    QPixmap scaledBrickImg = QPixmap::fromImage(brickImage).scaled(brickSize, brickSize, Qt::KeepAspectRatio);

    // Calculate the offset to center the map
    int offsetX = (width() - maze.getWidth() * brickSize) / 2;
    int offsetY = (height() - maze.getHeight() * brickSize) / 2 - 50;

    for (int i = 0; i < maze.getWidth(); ++i) {
        for (int j = 0; j < maze.getHeight(); ++j) {
            if (maze(i, j) == 1) {  // If the cell is a wall, create a brick
                BrickItem* brick = new BrickItem(nullptr);
                brick->setPixmap(scaledBrickImg);
                brick->setPos(offsetX + i * brick->pixmap().width(), offsetY + j * brick->pixmap().height());
                this->addItem(brick);
            }
        }
    }

    // Create the hero
    Hero* hero = new Hero();

    // Set the focus to the hero
    hero->setFocus();
    hero->setHeroImg(UIResource::kHero);
    // Add the hero to the scene
    this->addItem(hero);

    // Try to place the hero
    bool heroPlaced = false;
    while (!heroPlaced) {
        // Set the hero's position to a random place in the scene
        int x = QRandomGenerator::global()->bounded(20) * brickSize + offsetX;
        int y = QRandomGenerator::global()->bounded(10) * brickSize + offsetY;

        if (x < 200 || x>1500 || y < 200 || y>700) {
            continue;
        }
        hero->setPos(x, y);

        // Check if the hero is colliding with something
        if (hero->collidingItems().isEmpty()) {
            // If not, then the hero is successfully placed
            heroPlaced = true;
        }
    }


    // Create the monsters based on the difficulty level
    int monsterCount;
    switch (difficulty) {
    case Difficulty::EASY:
        monsterCount = 5;
        break;
    case Difficulty::MEDIUM:
        monsterCount = 10;
        break;
    case Difficulty::HARD:
        monsterCount = 15;
        break;
    default:
        monsterCount = 5;
        break;
    }

    for (int i = 0; i < monsterCount; i++) {
        Monster* monster = new Monster();
        monster->setMonsterImg(UIResource::kMonster);
        // Set the step size to be the same as the brick size
        monster->setStepSize(brickSize);

        // Add the monster to the scene
        this->addItem(monster);

        // Position the monster in a random location that doesn't collide with anything
        int x, y;
        do {
            x = QRandomGenerator::global()->bounded(20) * brickSize + offsetX;
            y = QRandomGenerator::global()->bounded(10) * brickSize + offsetY;
            monster->setPos(x, y);
        } while (!monster->collidingItems().isEmpty());

        // Setup a timer to move the monster every 1 second
        QTimer* timer = new QTimer();
        QObject::connect(timer, &QTimer::timeout, monster, &Monster::randomMove);
        timer->start(500);
    }



}
