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

  signals:


  private slots:

    /// @brief Slot called when a button is clicked.
    void startButtonClicked();

    /// @brief Slot called when a button is clicked
    void exitButtonClicked();

    /// @brief Slot called when a button is clicked
    void returnHomeClicked();

};
#endif  // MAINWINDOW_H
