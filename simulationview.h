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
#include "qopenglwidget.h"

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

    /// @brief Converts on screen pixels into corresponding box2d meters as determined by the pixels per meter variable.
    double pixelsToMeters(double pixels);
    /// @brief Converts on box2d meters into corresponding view pixels as determined by the pixels per meter variable.
    double metersToPixels(double meters);

    /// @brief The number of view pixels for every meter in box2d.
    const int pixelsPerMeter = 60;

    /// @brief Defines a box2d shape with corresponding graphics and stores it.
    void createShape(int shape);

    /// @brief The graphics scene that displays QGraphicsItems.
    QGraphicsScene* scene;
    /// @brief The OpenGl Widget for the Scene.
    QOpenGLWidget *gl;
    /// @brief The Box2D world.
    b2World world;
    /// @brief The dynamic bodies of the car objects
    QList<b2Body*> shapeBodies;
    /// @brief The car pieces to show on the screen
    QList<QGraphicsItem*> shapeImages;

    int                shapeCount         = 75;
    double             elasticity         = 1.0;
    double             friction           = 0.0;
    double             shapeDensity       = 0.50;
    int                shapeWidth         = 10;
    int                shapeHeight        = 10;
    int                shape              = random;
    bool               overrideShapeColor = false;
    QColor             shapeColor;
    QGraphicsPathItem* sceneBackground;

    /// @brief The timer that updates the Box2D world and the graphics scene.
    QTimer* timer;

    /// @brief Returns a random number in the given range.
    int getRandomNumber(int min, int max);

    /// @brief A set of arrow keys that have been pressed since the last world update.
    QSet<int> pressedKeys;

    /// @brief A list of forces that should be applied to each shape on the next world update.
    QList<b2Vec2> forces;

  signals:

    /// @brief Signals to the main window to display the ... animation on the simulation screen.
    void startLabelTimer(int milliseconds);
       /// @brief Signals to the main window to stop the ... animation on the simulation screen.
    void stopLabelTimer();

  private slots:

    /// @brief Applies a list of given forces to every body in the shapeBodies list.
    void applyForcesToAllBodies();

  public slots:

    /// @brief Sets the shape count for the simulation.
    void setShapeCount(int count);
    /// @brief Sets the shape bounce elasticity for the simulation.
    void setElasticity(double elasticity);
    /// @brief Sets the world gravity for the simulation.
    void setGravity(double gravity);
    /// @brief Sets the shape friction for the simulation.
    void setFriction(double friction);
    /// @brief Sets the shape density for the simulation.
    void setShapeDensity(double density);
    /// @brief Sets the shape width for the simulation.
    void setShapeWidth(int width);
    /// @brief Sets the shape height for the simulation.
    void setShapeHeight(int height);
    /// @brief Sets the background color for the simulation.
    void setBackgroundColor(bool override, QColor color);
    /// @brief Sets the shape color for the simulation.
    void setShapeColor(bool override, QColor color);
    /// @brief Sets the shape for the simulation.
    void setShape(int shape);
    /// @brief Starts the simulation.
    void runSimulation();
    /// @brief Stops the simulation.
    void stopSimulation();

  protected:

    /// @brief Overridden resizeEvent for QGraphicsView that scales its child scene upon resize.
    void resizeEvent(QResizeEvent* event) override;

    /// @brief Overriden keyPressEvent to handle left and right arrow key presses.
    bool eventFilter(QObject* obj, QEvent* event) override;

  public slots:

    /// @brief Called by the world timer, Steps the Box2D forward and updates the graphics scene accordingly.
    void updateWorld();

};

#endif  // SIMULATIONVIEW_H
