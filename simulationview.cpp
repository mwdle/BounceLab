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

    sceneBackground = new QGraphicsPathItem();
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, scene->width(), scene->height()), 10, 10);
    sceneBackground->setPath(path);
    sceneBackground->setBrush(Qt::black);
    scene->addItem(sceneBackground);
    setPosition(sceneBackground, scene->width() / 2, scene->height() / 2);

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

    // Setup the physics timer
    QWidget::setFocus();
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &SimulationView::updateWorld);
}

void SimulationView::updateWorld() {
    // Instruct the world to perform a single step of simulation.
    world.Step(1.0f / 60.0f, 6, 2);

    for(int i = 0; i < shapeCount; i++) {
        b2Vec2 position = shapeBodies.at(i)->GetPosition();
        setPosition(shapeImages.at(i), position.x, position.y);
    }
}

void SimulationView::setShapeCount(int count) {
    shapeCount = count;
}

void SimulationView::setShape(int shape) {
    this->shape = shape;
}

void SimulationView::setElasticity(double elasticity) {
    this->elasticity = elasticity;
}

void SimulationView::setGravity(double gravity) {
    world.SetGravity(b2Vec2(0.0f, gravity));
}

void SimulationView::setFriction(double friction) {
    this->friction = friction;
}

void SimulationView::setShapeDensity(double density) {
    this->shapeDensity = density;
}

void SimulationView::setShapeWidth(int width) {
    this->shapeWidth = width;
}

void SimulationView::setShapeHeight(int height) {
    this->shapeHeight = height;
}

void SimulationView::setBackgroundColor(bool override, QColor color) {
    if(override)
        sceneBackground->setBrush(color);
    else
        sceneBackground->setBrush(Qt::black);
}

void SimulationView::setShapeColor(bool override, QColor color) {
    overrideShapeColor = override;
    shapeColor         = color;
}

void SimulationView::setPosition(QGraphicsItem* itemToPosition, float x, float y) {
    itemToPosition->setPos(x - itemToPosition->boundingRect().width() / 2,
                           scene->height() - y - itemToPosition->boundingRect().height() / 2);
}

void SimulationView::resizeEvent(QResizeEvent* event) {
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void SimulationView::runSimulation() {
    timer->stop();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for(QGraphicsItem* shapeImage : shapeImages) {
        delete shapeImage;
    }
    shapeImages.clear();
    for(b2Body* shapeBody : shapeBodies) {
        world.DestroyBody(shapeBody);
    }
    shapeBodies.clear();
    for(int i = 0; i < shapeCount; i++) {

        b2CircleShape ballShape;
        ballShape.m_radius = shapeWidth / 2;
        // Define the car's dynamic body. We set its position and call the body factory.
        b2BodyDef ballDef;
        ballDef.type = b2_dynamicBody;
        ballDef.position.Set(getRandomNumber(0, scene->width()), getRandomNumber(0, scene->height()));
        b2Body* ballBody = world.CreateBody(&ballDef);

        // Define the dynamic body fixture.
        b2FixtureDef ballFixture;
        ballFixture.shape       = &ballShape;
        ballFixture.density     = shapeDensity;
        ballFixture.friction    = friction;
        ballFixture.restitution = elasticity;

        // Add the shape to the body.
        ballBody->CreateFixture(&ballFixture);
        shapeBodies.push_back(ballBody);
        b2Vec2 impulseForce =
            b2Vec2(((scene->width() / 2) - ballBody->GetPosition().x) * 50, ((scene->height() / 2) - ballBody->GetPosition().y) * 50);
        ballBody->ApplyLinearImpulse(impulseForce, ballBody->GetLocalCenter(), true);

        QGraphicsEllipseItem* ballImage = new QGraphicsEllipseItem(0, 0, ballShape.m_radius * 2, ballShape.m_radius * 2);
        if(overrideShapeColor)
            ballImage->setBrush(shapeColor);
        else
            ballImage->setBrush(QColor(getRandomNumber(0, 255), getRandomNumber(0, 255), getRandomNumber(0, 255)));
        scene->addItem(ballImage);
        shapeImages.push_back(ballImage);
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
    delete sceneBackground;
    for(b2Body* ballBody : shapeBodies) {
        if(ballBody != nullptr) {
            world.DestroyBody(ballBody);
        }
    }
    for(QGraphicsItem* ballImage : shapeImages) {
        delete ballImage;
    }
}
