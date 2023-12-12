/*
 * TEAM S.A.M.B.A.M:
 *        Adam Liu, Aidan Spendlove, Sam Hill,
 *        Blake England, Matthew Johnson, Michael Wadley
 *
 * GitHub: https://github.com/University-of-Utah-CS3505/A9EduApp
 * Project: Assignment 9 - Educational App
 * File: mainwindow.cpp
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRandomGenerator>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    /*
     * UI Setup Code
     */

    ui->setupUi(this);

    // Connect view buttons to controller and model

    connect(ui->startButton, &QPushButton::clicked, ui->simulationBox, &SimulationView::runSimulation);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::exitButtonClicked);
    connect(ui->returnHomeButton, &QPushButton::clicked, this, &MainWindow::returnHomeClicked);

    connect(ui->ballCount, &QSpinBox::valueChanged, ui->simulationBox, &SimulationView::setBallCount);
    connect(ui->bouncinessLevel, &QDoubleSpinBox::valueChanged, ui->simulationBox, &SimulationView::setBounciness);
    connect(ui->monochromeStatus, &QCheckBox::stateChanged, ui->simulationBox, &SimulationView::setMonochrome);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::returnHomeClicked() {
    ui->screenManager->setCurrentIndex(0);
}

void MainWindow::startButtonClicked() {
    ui->screenManager->setCurrentIndex(1);
}

void MainWindow::exitButtonClicked() {
    QApplication::quit();
}
