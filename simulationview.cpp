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
    scene = new QGraphicsScene(0, 0, 800, 546);
    setScene(scene);
    scene->setBackgroundBrush(QBrush(QColor(0, 0, 0)));

    // BOX2D SETUP

    // Define the ground.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(scene->width() / 2, 0.0f);
    b2Body*        groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(scene->width() / 2, 0.0f);
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
    leftWallBodyDef.position.Set(0.0f, scene->height() / 2);
    b2Body*        leftWallBody = world.CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(0.0f, scene->height() / 2);
    leftWallBody->CreateFixture(&leftWallBox, 0.0f);

    // Define the right wall.
    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(scene->width(), scene->height() / 2);
    b2Body*        rightWallBody = world.CreateBody(&rightWallBodyDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(0.0f, scene->height() / 2);
    rightWallBody->CreateFixture(&rightWallBox, 0.0f);

    leftSpaceFiller = new QGraphicsRectItem(0, 0, scene->width(), scene->height() * 2);
    leftSpaceFiller->setBrush(QColor(50, 50, 50));
    leftSpaceFiller->setPen(QPen(QColor(50, 50, 50)));
    scene->addItem(leftSpaceFiller);
    setPosition(leftSpaceFiller, -(scene->width() / 2), (scene->height() / 2));

    rightSpaceFiller = new QGraphicsRectItem(0, 0, scene->width(), scene->height() * 2);
    rightSpaceFiller->setBrush(QColor(50, 50, 50));
    rightSpaceFiller->setPen(QPen(QColor(50, 50, 50)));
    scene->addItem(rightSpaceFiller);
    setPosition(rightSpaceFiller, scene->width() + (scene->width() / 2), (scene->height() / 2));

    topSpaceFiller = new QGraphicsRectItem(0, 0, scene->width() * 2, scene->height());
    topSpaceFiller->setBrush(QColor(50, 50, 50));
    topSpaceFiller->setPen(QPen(QColor(50, 50, 50)));
    scene->addItem(topSpaceFiller);
    setPosition(topSpaceFiller, (scene->width() / 2), scene->height() + (scene->height() / 2));

    bottomSpaceFiller = new QGraphicsRectItem(0, 0, scene->width() * 2, scene->height());
    bottomSpaceFiller->setBrush(QColor(50, 50, 50));
    bottomSpaceFiller->setPen(QPen(QColor(50, 50, 50)));
    scene->addItem(bottomSpaceFiller);
    setPosition(bottomSpaceFiller, (scene->width() / 2), -(scene->height() / 2));

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

void SimulationView::setBallRadius(int pixels) {
    ballRadius = pixels;
}
void SimulationView::setBackgroundColor(bool override, QColor color) {
    if(override)
        scene->setBackgroundBrush(color);
    else
        scene->setBackgroundBrush(Qt::black);
}
void SimulationView::setBallColor(bool override, QColor color) {
    overrideBallColor = override;
    ballColor         = color;
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
        ballShape.m_radius = ballRadius;
        // Define the car's dynamic body. We set its position and call the body factory.
        b2BodyDef ballDef;
        ballDef.type = b2_dynamicBody;
        ballDef.position.Set(getRandomNumber(0, scene->width()), getRandomNumber(0, scene->height()));
        b2Body* ballBody = world.CreateBody(&ballDef);

        // Define the dynamic body fixture.
        b2FixtureDef ballFixture;
        ballFixture.shape       = &ballShape;
        ballFixture.density     = 1.0f;
        ballFixture.friction    = 0.0f;
        ballFixture.restitution = restitution;

        // Add the shape to the body.
        ballBody->CreateFixture(&ballFixture);
        ballBodies.push_back(ballBody);
        b2Vec2 impulseForce =
            b2Vec2(((scene->width() / 2) - ballBody->GetPosition().x) * 50, ((scene->height() / 2) - ballBody->GetPosition().y) * 50);
        ballBody->ApplyLinearImpulse(impulseForce, ballBody->GetLocalCenter(), true);

        QGraphicsEllipseItem* ballImage = new QGraphicsEllipseItem(0, 0, ballShape.m_radius * 2, ballShape.m_radius * 2);
        if(overrideBallColor)
            ballImage->setBrush(ballColor);
        else if(monochrome)
            ballImage->setBrush(Qt::white);
        else
            ballImage->setBrush(QColor(getRandomNumber(0, 255), getRandomNumber(0, 255), getRandomNumber(0, 255)));
        scene->addItem(ballImage);
        ballImages.push_back(ballImage);
        setPosition(ballImage, ballBody->GetPosition().x, ballBody->GetPosition().y);
    }
    timer->start(3);
}

int SimulationView::getRandomNumber(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

void SimulationView::stopSimulation() {
    timer->stop();
}

SimulationView::~SimulationView() {
    timer->stop();
    delete timer;
    delete scene;
    delete leftSpaceFiller;
    delete rightSpaceFiller;
    delete topSpaceFiller;
    delete bottomSpaceFiller;
    for (b2Body* ballBody : ballBodies) {
        if(ballBody != nullptr) {
            world.DestroyBody(ballBody);
        }
    }
    for (QGraphicsItem* ballImage : ballImages) {
        delete ballImage;
    }
}
