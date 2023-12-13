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

    connect(ui->shapeColor, &QCheckBox::clicked, this, &MainWindow::shapeColorOverride);
    connect(this, &MainWindow::updateShapeColor, ui->simulationBox, &SimulationView::setShapeColor);

    connect(ui->backgroundColor, &QCheckBox::clicked, this, &MainWindow::backgroundColorOverride);
    connect(this, &MainWindow::updateBackgroundColor, ui->simulationBox, &SimulationView::setBackgroundColor);

    connect(ui->shapeWidth, &QSpinBox::valueChanged, ui->simulationBox, &SimulationView::setShapeWidth);
    connect(ui->shapeHeight, &QSpinBox::valueChanged, ui->simulationBox, &SimulationView::setShapeHeight);
    connect(ui->circleRadius, &QSpinBox::valueChanged, this, &MainWindow::setCircleRadius);
    connect(this, &MainWindow::updateCircleRadius, ui->simulationBox, &SimulationView::setShapeWidth);
    connect(this, &MainWindow::updateCircleRadius, ui->simulationBox, &SimulationView::setShapeHeight);

    connect(ui->shapeDropdown, &QComboBox::currentIndexChanged, ui->simulationBox, &SimulationView::setShape);

    connect(ui->shapeCount, &QSpinBox::valueChanged, ui->simulationBox, &SimulationView::setShapeCount);
    connect(ui->shapeDensity, &QDoubleSpinBox::valueChanged, ui->simulationBox, &SimulationView::setShapeDensity);
    connect(ui->elasticity, &QDoubleSpinBox::valueChanged, ui->simulationBox, &SimulationView::setElasticity);
    connect(ui->friction, &QDoubleSpinBox::valueChanged, ui->simulationBox, &SimulationView::setFriction);
    connect(ui->gravity, &QDoubleSpinBox::valueChanged, ui->simulationBox, &SimulationView::setGravity);

    connect(ui->generalSettingsButton, &QPushButton::clicked, this, &MainWindow::generalSettingsClicked);
    connect(ui->shapeSettingsButton, &QPushButton::clicked, this, &MainWindow::shapeSettingsClicked);
    connect(ui->colorSettingsButton, &QPushButton::clicked, this, &MainWindow::colorSettingsClicked);

    connect(ui->shapeDropdown, &QComboBox::activated, this, &MainWindow::shapeSelected);
}

void MainWindow::shapeSelected(int shape) {
    switch(shape) {
        case 0: ui->shapeSpecificSettings->setCurrentIndex(0); break;
        case 1: ui->shapeSpecificSettings->setCurrentIndex(1); break;
        case 2: ui->shapeSpecificSettings->setCurrentIndex(2); break;
        case 3: ui->shapeSpecificSettings->setCurrentIndex(2); break;
    }
}

void MainWindow::setCircleRadius(int radius) {
    emit updateCircleRadius(radius * 2);
}

void MainWindow::shapeColorOverride(bool override) {
    if(override) {
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");
        if(color.isValid()) {
            emit updateShapeColor(true, color);
            ui->shapeColor->setStyleSheet(
                "QCheckBox::indicator:checked { background-color : " + color.name() +
                "; border-style:inset; border-color:gray; border-width: 1px; border-radius:3px; width: 15px; height: 15px; }");
        } else {
            ui->shapeColor->setCheckState(Qt::Unchecked);
            ui->shapeColor->setStyleSheet("");
        }
    } else {
        emit updateShapeColor(false, Qt::white);
        ui->shapeColor->setStyleSheet("");
    }
}

void MainWindow::backgroundColorOverride(bool override) {
    if(override) {
        QColor color = QColorDialog::getColor(Qt::black, nullptr, "Select Color");
        if(color.isValid()) {
            emit updateBackgroundColor(true, color);
            ui->backgroundColor->setStyleSheet(
                "QCheckBox::indicator:checked { background-color : " + color.name() +
                "; border-style:inset; border-color:gray; border-width: 1px; border-radius:3px; width: 15px; height: 15px; }");
        } else {
            ui->backgroundColor->setCheckState(Qt::Unchecked);
            ui->backgroundColor->setStyleSheet("");
        }
    } else {
        emit updateBackgroundColor(false, Qt::black);
        ui->backgroundColor->setStyleSheet("");
    }
}

void MainWindow::generalSettingsClicked() {
    ui->settings->setCurrentIndex(0);
}

void MainWindow::shapeSettingsClicked() {
    ui->settings->setCurrentIndex(1);
}

void MainWindow::colorSettingsClicked() {
    ui->settings->setCurrentIndex(2);
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
