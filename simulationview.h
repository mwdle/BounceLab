/*
 * TEAM S.A.M.B.A.M:
 *        Adam Liu, Aidan Spendlove, Sam Hill,
 *        Blake England, Matthew Johnson, Michael Wadley
 *
 * GitHub: https://github.com/University-of-Utah-CS3505/A9EduApp
 * Project: Assignment 9 - Educational App
 * File: simulationview.h
 */

#ifndef SIMULATIONVIEW_H
#define SIMULATIONVIEW_H

#include "Box2D/Dynamics/b2World.h"
#include "qgraphicsitem.h"

#include <QGraphicsView>

using std::string;

class SimulationView: public QGraphicsView {
    Q_OBJECT

  public:
    SimulationView(QWidget* parent = nullptr);
    ~SimulationView();

    enum Shapes {
        random    = 0,
        circle    = 1,
        rectangle = 2,
        triangle  = 3,
    };

  private:
    /// @brief Converts Box2D coordinates to scene coordinates and sets the position of the parameter QGraphicsItem in the scene.
    /// @param itemToPosition The GraphicsItem to position on the scene.
    /// @param x The Box2D x position to convert to scene coordinates.
    /// @param y The Box2D y position to convert to scene coordinates.
    void setPosition(QGraphicsItem* itemToPosition, float x, float y);

    /// @brief Defines a shape and stores it.
    void createShape(int shape);

    /// @brief The graphics scene that displays QGraphicsItems.
    QGraphicsScene* scene;
    /// @brief The Box2D world.
    b2World world;
    /// @brief The dynamic bodies of the car objects
    QList<b2Body*> shapeBodies;
    /// @brief The car pieces to show on the screen
    QList<QGraphicsItem*> shapeImages;

    int                shapeCount         = 50;
    double             elasticity         = 1.0;
    double             friction           = 0.0;
    double             shapeDensity       = 1.0;
    int                shapeWidth         = 10;
    int                shapeHeight        = 10;
    int                shape              = random;
    bool               overrideShapeColor = false;
    QColor             shapeColor;
    QGraphicsPathItem* sceneBackground;

    /// @brief The timer that updates the Box2D world and the graphics scene.
    QTimer* timer;

    int getRandomNumber(int min, int max);

  signals:


  private slots:

  public slots:
    void setShapeCount(int count);
    void setElasticity(double elasticity);
    void setGravity(double gravity);
    void setFriction(double friction);
    void setShapeDensity(double density);
    void setShapeWidth(int width);
    void setShapeHeight(int height);
    void setBackgroundColor(bool override, QColor color);
    void setShapeColor(bool override, QColor color);
    void setShape(int shape);
    void runSimulation();
    void stopSimulation();

  protected:

    /// @brief Overridden resizeEvent for QGraphicsView that scales its child scene upon resize.
    void resizeEvent(QResizeEvent* event) override;

  public slots:

    /// @brief Called by the world timer, Steps the Box2D forward and updates the graphics scene accordingly.
    void updateWorld();

};

#endif  // SIMULATIONVIEW_H
