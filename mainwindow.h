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

  signals:
    void updateShapeColor(bool override, QColor color);
    void updateBackgroundColor(bool override, QColor color);
    void updateCircleRadius(int width);

  public slots:

    /// @brief Slot called when a simulation begins.
    void updateSimulationLabel();

  private slots:

    void startLabelTimer(int milliseconds);
    void stopLabelTimer();
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

    /// @brief Slot called when a button is clicked
    void shapeSelected(int shape);

    /// @brief Slot called when a spinbox is changed
    void setCircleRadius(int radius);

    /// @brief Slot called when a checkbox is selected
    void shapeColorOverride(bool override);

    /// @brief Slot called when a checkbox is selected
    void backgroundColorOverride(bool override);
};
#endif  // MAINWINDOW_H
