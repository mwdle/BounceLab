/*
 * TEAM S.A.M.B.A.M:
 *        Adam Liu, Aidan Spendlove, Sam Hill,
 *        Blake England, Matthew Johnson, Michael Wadley
 *
 * GitHub: https://github.com/University-of-Utah-CS3505/A9EduApp
 * Project: Assignment 9 - Educational App
 * File: simulationview.h
 */

#include "simulationview.h"

#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "qtimer.h"

#include <QKeyEvent>

SimulationView::SimulationView(QWidget* parent) : world(b2Vec2(0.0f, 0.0f)) {

    // SCENE SETUP
    ballCount = 30;
    restitution = 1.0;
    monochrome = false;

    scene = new QGraphicsScene(0, 0, 800, 442);
    setScene(scene);
    scene->setBackgroundBrush(QBrush(QColor(0, 0, 0)));

    // BOX2D SETUP

    // Define the ground.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(scene->width() / 2, 0.0f);
    b2Body*        groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(scene->width(), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the ceiling.
    b2BodyDef ceilingBodyDef;
    ceilingBodyDef.position.Set(scene->width() / 2, scene->height());
    b2Body*        ceilingBody = world.CreateBody(&ceilingBodyDef);
    b2PolygonShape ceilingBox;
    ceilingBox.SetAsBox(scene->width() / 2, 0.0f);
    ceilingBody->CreateFixture(&ceilingBox, 0.0f);

    // Define the left wall.
    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(0.0f, 0.0f);
    b2Body*        leftWallBody = world.CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(0.0f, scene->height() / 2);
    leftWallBody->CreateFixture(&leftWallBox, 0.0f);

    // Setup the physics timer
    QWidget::setFocus();
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &SimulationView::updateWorld);
}

void SimulationView::updateWorld() {
    // Instruct the world to perform a single step of simulation.
    world.Step(1.0f / 60.0f, 6, 2);

    for (int i = 0; i < ballCount; i++) {
        b2Vec2 position = ballBodies.at(i)->GetPosition();
        setPosition(ballImages.at(i), position.x, position.y);
    }
}

void SimulationView::setBallCount(int count) {
    ballCount = count;
}

void SimulationView::setBounciness(double bounciness) {
    restitution = bounciness;
}

void SimulationView::setMonochrome(bool status) {
    monochrome = status;
}

void SimulationView::setPosition(QGraphicsItem* itemToPosition, float x, float y) {
    itemToPosition->setPos(x - itemToPosition->boundingRect().width() / 2,
                           scene->height() - y - itemToPosition->boundingRect().height() / 2);
}

void SimulationView::resizeEvent(QResizeEvent* event) {
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void SimulationView::paintEvent(QPaintEvent* event) {
    QPainterPath path;
    path.addRoundedRect(QRectF(rect()), 10.0, 10.0, Qt::AbsoluteSize);
    QRegion mask = QRegion(path.toFillPolygon().toPolygon());
    setMask(mask);
    QGraphicsView::paintEvent(event);
}

void SimulationView::runSimulation() {
    timer->stop();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (QGraphicsItem* item : ballImages) {
        delete item;
    }
    ballImages.clear();
    for (b2Body* ballBody : ballBodies)
    {
        world.DestroyBody(ballBody);
    }
    ballBodies.clear();
    for (int i = 0; i < ballCount; i++) {
        b2CircleShape ballShape;
        ballShape.m_radius = 1.0f;
        // Define the car's dynamic body. We set its position and call the body factory.
        b2BodyDef ballDef;
        ballDef.type = b2_dynamicBody;
        ballDef.position.Set((scene->width() / 2) + getRandomNumber(0, 400), (scene->height() / 2) + getRandomNumber(0, 221));
        b2Body* ballBody = world.CreateBody(&ballDef);

        // Define the dynamic body fixture.
        b2FixtureDef ballFixture;
        ballFixture.shape       = &ballShape;
        ballFixture.density     = 1.0f;
        ballFixture.friction    = 0.1f;
        ballFixture.restitution = restitution;

        // Add the shape to the body.
        ballBody->CreateFixture(&ballFixture);
        ballBodies.push_back(ballBody);

        QGraphicsEllipseItem* ballImage = new QGraphicsEllipseItem(0,0,10,10);
        ballImage->setBrush(QColor(getRandomNumber(0, 255), getRandomNumber(0,255), getRandomNumber(0, 255)));
        scene->addItem(ballImage);
        ballImages.push_back(ballImage);
        setPosition(ballImage, ballBody->GetPosition().x, ballBody->GetPosition().y);
    }
    timer->start(3);
}

int SimulationView::getRandomNumber(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

SimulationView::~SimulationView() {
    timer->stop();
    delete timer;
    delete scene;
    for (b2Body* ballBody : ballBodies) {
        if(ballBody != nullptr) {
            world.DestroyBody(ballBody);
        }
    }
    for (QGraphicsItem* ballImage : ballImages) {
        delete ballImage;
    }
}
