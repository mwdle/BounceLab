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

  private:
    /// @brief Converts Box2D coordinates to scene coordinates and sets the position of the parameter QGraphicsItem in the scene.
    /// @param itemToPosition The GraphicsItem to position on the scene.
    /// @param x The Box2D x position to convert to scene coordinates.
    /// @param y The Box2D y position to convert to scene coordinates.
    void setPosition(QGraphicsItem* itemToPosition, float x, float y);

    /// @brief The graphics scene that displays QGraphicsItems.
    QGraphicsScene* scene;
    /// @brief The Box2D world.
    b2World world;
    /// @brief The dynamic bodies of the car objects
    QList<b2Body*> ballBodies;
    /// @brief The car pieces to show on the screen
    QList<QGraphicsItem*> ballImages;

    int ballCount;
    double restitution;
    bool monochrome;

    /// @brief The timer that updates the Box2D world and the graphics scene.
    QTimer* timer;

    int getRandomNumber(int min, int max);

  signals:


  private slots:

  public slots:
    void setBallCount(int count);
    void setBounciness(double bounciness);
    void setMonochrome(bool status);
    void runSimulation();

  protected:

    /// @brief Overridden resizeEvent for QGraphicsView that scales its child scene upon resize.
    void resizeEvent(QResizeEvent* event) override;
    /// @brief Overriden paintEvent to handle round the corners of the QGraphicsView.
    void paintEvent(QPaintEvent* event) override;

  public slots:

    /// @brief Called by the world timer, Steps the Box2D forward and updates the graphics scene accordingly.
    void updateWorld();

};

#endif  // SIMULATIONVIEW_H
