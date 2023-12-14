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

SimulationView::SimulationView(QWidget* parent): world(b2Vec2(0.0f, 0.0f)) {

    scene = new QGraphicsScene(0, 0, 800, 546);
    setScene(scene);

    sceneBackground = new QGraphicsPathItem();
    QPainterPath path;
    path.addRoundedRect(QRectF(0, 0, scene->width(), scene->height()), 10, 10);
    sceneBackground->setPath(path);
    sceneBackground->setBrush(Qt::black);
    scene->addItem(sceneBackground);
    setPosition(sceneBackground, scene->width() / 2, scene->height() / 2);

    this->installEventFilter(this);

    // Define the ground.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(pixelsToMeters(scene->width() / 2), 0.0f);
    b2Body*        groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(pixelsToMeters(scene->width() / 2), 0.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    // Define the ceiling.
    b2BodyDef ceilingBodyDef;
    ceilingBodyDef.position.Set(pixelsToMeters(scene->width() / 2), pixelsToMeters(scene->height()));
    b2Body*        ceilingBody = world.CreateBody(&ceilingBodyDef);
    b2PolygonShape ceilingBox;
    ceilingBox.SetAsBox(pixelsToMeters(scene->width() / 2), 0.0f);
    ceilingBody->CreateFixture(&ceilingBox, 0.0f);

    // Define the left wall.
    b2BodyDef leftWallBodyDef;
    leftWallBodyDef.position.Set(0.0f, pixelsToMeters(scene->height() / 2));
    b2Body*        leftWallBody = world.CreateBody(&leftWallBodyDef);
    b2PolygonShape leftWallBox;
    leftWallBox.SetAsBox(0.0f, pixelsToMeters(scene->height() / 2));
    leftWallBody->CreateFixture(&leftWallBox, 0.0f);

    // Define the right wall.
    b2BodyDef rightWallBodyDef;
    rightWallBodyDef.position.Set(pixelsToMeters(scene->width()), pixelsToMeters(scene->height()) / 2);
    b2Body*        rightWallBody = world.CreateBody(&rightWallBodyDef);
    b2PolygonShape rightWallBox;
    rightWallBox.SetAsBox(0.0f, pixelsToMeters(scene->height() / 2));
    rightWallBody->CreateFixture(&rightWallBox, 0.0f);

    // Setup the physics timer
    QWidget::setFocus();
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &SimulationView::updateWorld);
}

void SimulationView::updateWorld() {
    // Instruct the world to perform a single step of simulation.
    world.Step(1.0f / 90.0f, 8, 3);

    if(pressedKeys.contains(Qt::Key_Up))
        forces.push_back(b2Vec2(0, 0.2f));
    if(pressedKeys.contains(Qt::Key_Down))
        forces.push_back(b2Vec2(0, -0.2f));
    if(pressedKeys.contains(Qt::Key_Left))
        forces.push_back(b2Vec2(-0.2f, 0));
    if(pressedKeys.contains(Qt::Key_Right))
        forces.push_back(b2Vec2(0.2f, 0));

    applyForcesToAllBodies();

    for(int i = 0; i < shapeCount; i++) {
        b2Vec2 position = shapeBodies.at(i)->GetPosition();
        setPosition(shapeImages.at(i), metersToPixels(position.x), metersToPixels(position.y));
        float32 angleInDegrees = shapeBodies.at(i)->GetAngle() * (180.0 / M_PI);
        shapeImages.at(i)->setRotation(angleInDegrees);
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
        if(shape == random)
            createShape(getRandomNumber(1, 2));
        else
            createShape(shape);
    }
    timer->start(11);
    emit startLabelTimer(500);
}

void SimulationView::createShape(int shape) {
    switch(shape) {
        case circle: {
            b2CircleShape ballShape;
            ballShape.m_radius = pixelsToMeters(shapeWidth / 2);
            // Define the car's dynamic body. We set its position and call the body factory.
            b2BodyDef ballDef;
            ballDef.type = b2_dynamicBody;
            ballDef.position.Set(getRandomNumber(pixelsToMeters(60), pixelsToMeters(scene->width() - 60)),
                                 getRandomNumber(pixelsToMeters(60), pixelsToMeters(scene->height() - 60)));
            ballDef.angle      = 0.0f * b2_pi;
            ballDef.allowSleep = true;
            ballDef.awake      = true;
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

            QGraphicsEllipseItem* ballImage =
                new QGraphicsEllipseItem(0, 0, metersToPixels(ballShape.m_radius * 2), metersToPixels(ballShape.m_radius * 2));
            ballImage->setTransformOriginPoint(QPoint(ballImage->boundingRect().width() / 2, ballImage->boundingRect().height() / 2));

            if(overrideShapeColor)
                ballImage->setBrush(shapeColor);
            else
                ballImage->setBrush(QColor(getRandomNumber(0, 255), getRandomNumber(0, 255), getRandomNumber(0, 255)));

            scene->addItem(ballImage);
            shapeImages.push_back(ballImage);
            setPosition(ballImage, metersToPixels(ballBody->GetPosition().x), metersToPixels(ballBody->GetPosition().y));
            break;
        }

        case rectangle: {
            b2BodyDef rectDef;
            rectDef.type = b2_dynamicBody;
            rectDef.position.Set(getRandomNumber(pixelsToMeters(60), pixelsToMeters(scene->width() - 60)),
                                 getRandomNumber(pixelsToMeters(60), pixelsToMeters(scene->height() - 60)));
            b2Body*        rectBody = world.CreateBody(&rectDef);
            rectDef.angle           = 0.0f * b2_pi;
            rectDef.allowSleep      = true;
            rectDef.awake           = true;
            b2PolygonShape dynamicBox;
            dynamicBox.SetAsBox(pixelsToMeters(shapeWidth / 2), pixelsToMeters(shapeHeight / 2));

            // Define the dynamic body fixture.
            b2FixtureDef rectFixture;
            rectFixture.shape       = &dynamicBox;
            rectFixture.density     = shapeDensity;
            rectFixture.friction    = friction;
            rectFixture.restitution = elasticity;

            // Add the shape to the body.
            rectBody->CreateFixture(&rectFixture);
            shapeBodies.push_back(rectBody);

            QGraphicsRectItem* rectImage = new QGraphicsRectItem(0, 0, shapeWidth, shapeHeight);
            rectImage->setTransformOriginPoint(QPoint(rectImage->boundingRect().width() / 2, rectImage->boundingRect().height() / 2));
            scene->addItem(rectImage);
            shapeImages.push_back(rectImage);
            if(overrideShapeColor)
                rectImage->setBrush(shapeColor);
            else
                rectImage->setBrush(QColor(getRandomNumber(0, 255), getRandomNumber(0, 255), getRandomNumber(0, 255)));
            setPosition(rectImage, metersToPixels(rectBody->GetPosition().x), metersToPixels(rectBody->GetPosition().y));
            break;
        }

        case triangle: {
            b2BodyDef triangleDef;
            triangleDef.type = b2_dynamicBody;
            triangleDef.position.Set(getRandomNumber(pixelsToMeters(60), pixelsToMeters(scene->width() - 60)),
                                     getRandomNumber(pixelsToMeters(60), pixelsToMeters(scene->height() - 60)));
            triangleDef.angle      = 0.0f * b2_pi;
            triangleDef.allowSleep = true;
            triangleDef.awake      = true;
            b2Body* triangleBody = world.CreateBody(&triangleDef);
            b2Vec2  vertices[3];
            vertices[0].Set(0.0f, 0.0f);
            vertices[1].Set(pixelsToMeters(shapeWidth / 2), 0);
            vertices[2].Set(pixelsToMeters(shapeWidth / 2), pixelsToMeters(shapeHeight));
            b2PolygonShape triangleShape;
            triangleShape.Set(vertices, 3);

            // Define the dynamic body fixture.
            b2FixtureDef triangleFixture;
            triangleFixture.shape       = &triangleShape;
            triangleFixture.density     = shapeDensity;
            triangleFixture.friction    = friction;
            triangleFixture.restitution = elasticity;

            // Add the shape to the body.
            triangleBody->CreateFixture(&triangleFixture);
            shapeBodies.push_back(triangleBody);

            QPolygonF triangle;
            triangle << QPointF(0.0f, 0.0f) << QPointF(shapeWidth / 2, shapeHeight) << QPointF(shapeWidth, 0.0f);
            QGraphicsPolygonItem* triangleImage = scene->addPolygon(triangle);
            triangleImage->setTransformOriginPoint(
                QPoint(triangleImage->boundingRect().width() / 2, triangleImage->boundingRect().height() / 2));
            if(overrideShapeColor)
                triangleImage->setBrush(shapeColor);
            else
                triangleImage->setBrush(QColor(getRandomNumber(0, 255), getRandomNumber(0, 255), getRandomNumber(0, 255)));

            shapeImages.push_back(triangleImage);
            setPosition(triangleImage, metersToPixels(triangleBody->GetPosition().x), metersToPixels(triangleBody->GetPosition().y));
            break;
        }
    }
    b2Vec2 impulseForce = b2Vec2((pixelsToMeters((scene->width() / 2)) - shapeBodies.at(shapeBodies.length() - 1)->GetPosition().x) * 4,
                                 (pixelsToMeters((scene->height() / 2)) - shapeBodies.at(shapeBodies.length() - 1)->GetPosition().y) * 4);
    shapeBodies.at(shapeBodies.length() - 1)->ApplyForceToCenter(impulseForce, true);
    // shapeBodies.at(shapeBodies.length() - 1)->ApplyForceToCenter(b2Vec2(0, -5000), true);
}

double SimulationView::pixelsToMeters(double pixels) {
    return pixels / pixelsPerMeter;
}

double SimulationView::metersToPixels(double meters) {
    return meters * pixelsPerMeter;
}

int SimulationView::getRandomNumber(int min, int max) {
    return min + std::rand() % (max - min + 1);
}

bool SimulationView::eventFilter(QObject* obj, QEvent* event) {
    if(event->type() == QEvent::KeyPress)
        pressedKeys += ((QKeyEvent*) event)->key();
    else if(event->type() == QEvent::KeyRelease)
        pressedKeys -= ((QKeyEvent*) event)->key();
    return false;
}

void SimulationView::applyForcesToAllBodies() {
    for(b2Body* body : shapeBodies) {
        for(b2Vec2 force : forces) {
            body->ApplyForceToCenter(force, true);
        }
    }
    forces.clear();
}

void SimulationView::stopSimulation() {
    timer->stop();
    emit stopLabelTimer();
}

SimulationView::~SimulationView() {
    timer->stop();
    delete timer;
    for(b2Body* ballBody : shapeBodies) {
        if(ballBody != nullptr) {
            world.DestroyBody(ballBody);
        }
    }
    for(QGraphicsItem* ballImage : shapeImages) {
        delete ballImage;
    }
    delete sceneBackground;
    delete scene;
}
