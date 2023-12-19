/*
 * TEAM S.A.M.B.A.M:
 *        Adam Liu, Aidan Spendlove, Sam Hill,
 *        Blake England, Matthew Johnson, Michael Wadley
 *
 * GitHub: https://github.com/University-of-Utah-CS3505/A9EduApp
 * Project: Assignment 9 - Educational App
 * File: mainwindow.h
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Box2D/Common/b2Math.h"

#include <QMainWindow>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private:
    /// @brief The UI object
    Ui::MainWindow* ui;
    /// @brief Controls dynamic in progress label.
    QTimer* labelTimer;
    QString buttonSelectedStylesheet =
        "QPushButton{ background-color: #3f424a; border-style: solid; border-color: black; border-width: 1px; border-radius:0px; } "
        "QPushButton:hover { background-color: #3f424a; } QPushButton:hover:pressed { background-color: #5f6470; }";
    QString buttonDeselectedStylesheet =
        "QPushButton{ background-color: #25272b; border-style: solid; border-color: black; border-width: 1px; border-radius:0px; } "
        "QPushButton:hover { background-color: #3f424a; } QPushButton:hover:pressed { background-color: #5f6470; }";

    QString colorCheckboxDefaultStyle = "QCheckBox { color: gray; } QCheckBox::indicator:unchecked { border-style:inset; border-color:gray; "
                                        "border-width: 1px; border-radius:3px; width: 25px; height: 25px; }";
  signals:

    /// @brief Signals to the simulation to update the shape color.
    void updateShapeColor(bool override, QColor color);
    /// @brief Signals to the simulation to update the background color.
    void updateBackgroundColor(bool override, QColor color);
    /// @brief Signals to the simulation to update the circle radius.
    void updateCircleRadius(int width);

  public slots:

    /// @brief Slot called when a simulation begins.
    void updateSimulationLabel();

  private slots:

    /// @brief Display the ... animation on the simulation screen.
    void startLabelTimer(int milliseconds);
    /// @brief Stop the ... animation on the simulation screen.
    void stopLabelTimer();
    /// @brief Slot called when a button is clicked.
    void returnToSettingsButtonClicked();
    /// @brief Slot called when a button is clicked.
    void startButtonClicked();
    /// @brief Slot called when a button is clicked
    void exitButtonClicked();
    /// @brief Slot called when a button is clicked
    void returnHomeClicked();
    /// @brief Slot called when a button is clicked
    void generalSettingsClicked();
    /// @brief Slot called when a button is clicked
    void shapeSettingsClicked();
    /// @brief Slot called when a button is clicked
    void colorSettingsClicked();
    /// @brief Slot called when a dropdown option is selected
    void shapeSelected(int shape);
    /// @brief Slot called when a spinbox is changed
    void setCircleRadius(int radius);
    /// @brief Slot called when a checkbox is selected
    void shapeColorOverride(bool override);
    /// @brief Slot called when a checkbox is selected
    void backgroundColorOverride(bool override);
};
#endif  // MAINWINDOW_H
