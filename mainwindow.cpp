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

#include <QColorDialog>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    /*
     * UI Setup Code
     */

    ui->setupUi(this);

    // Connect view buttons to controller and model

    connect(ui->startButton, &QPushButton::clicked, ui->simulationBox, &SimulationView::runSimulation);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startButtonClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::exitButtonClicked);
    connect(ui->exitButton, &QPushButton::clicked, ui->simulationBox, &SimulationView::stopSimulation);
    connect(ui->returnHomeButton, &QPushButton::clicked, this, &MainWindow::returnHomeClicked);
    connect(ui->returnHomeButton, &QPushButton::clicked, ui->simulationBox, &SimulationView::stopSimulation);

    connect(ui->ballColorCheckbox, &QCheckBox::clicked, this, &MainWindow::ballColorOverride);
    connect(this, &MainWindow::updateBallColor, ui->simulationBox, &SimulationView::setBallColor);

    connect(ui->backgroundColorCheckbox, &QCheckBox::clicked, this, &MainWindow::backgroundColorOverride);
    connect(this, &MainWindow::updateBackgroundColor, ui->simulationBox, &SimulationView::setBackgroundColor);

    connect(ui->ballRadius, &QSpinBox::valueChanged, ui->simulationBox, &SimulationView::setBallRadius);

    connect(ui->ballCount, &QSpinBox::valueChanged, ui->simulationBox, &SimulationView::setBallCount);
    connect(ui->bouncinessLevel, &QDoubleSpinBox::valueChanged, ui->simulationBox, &SimulationView::setBounciness);
}

void MainWindow::ballColorOverride(bool override) {
    if(override) {
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");
        if(color.isValid())
            emit updateBallColor(true, color);
        else
            ui->ballColorCheckbox->setCheckState(Qt::Unchecked);
    } else
        emit updateBallColor(false, Qt::white);
}

void MainWindow::backgroundColorOverride(bool override) {
    if(override) {
        QColor color = QColorDialog::getColor(Qt::black, nullptr, "Select Color");
        if(color.isValid())
            emit updateBackgroundColor(true, color);
        else
            ui->backgroundColorCheckbox->setCheckState(Qt::Unchecked);
    } else
        emit updateBackgroundColor(false, Qt::black);
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
