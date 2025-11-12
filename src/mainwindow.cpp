/*
 * @Author: love-yuri yuri2078170658@gmail.com
 * @Date: 2023-09-27 13:27:55
 * @LastEditTime: 2025-11-11 21:18:38
 * @Description:
 */

#include "mainwindow.h"
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