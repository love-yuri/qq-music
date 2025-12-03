#include "mainwindow.hpp"
#include "ui/ui_mainwindow.h"
#include <QMainWindow>

MainWindow::MainWindow(QMainWindow *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  show();
}

MainWindow::~MainWindow() {
  delete ui;
}