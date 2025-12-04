#include "mainwindow.hpp"
#include "ui/ui_mainwindow.h"
#include <QMainWindow>
#include <QFile>
#include "yuri_log.hpp"

MainWindow::MainWindow(QMainWindow *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QFile file(":/qss/main.qss");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    setStyleSheet(styleSheet() + file.readAll());
  }

  show();
}

MainWindow::~MainWindow() {
  delete ui;
}