#include "mainwindow.hpp"
#include "ui/ui_mainwindow.h"
#include <QMainWindow>
#include <QFile>
#include <QPainter>
#include "yuri_log.hpp"
#include "components/nav_menu_item.hpp"
#include <QTimer>

MainWindow::MainWindow(QMainWindow *parent) :
  QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  if (QFile file(":/qss/main.qss"); file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    setStyleSheet(styleSheet() + file.readAll());
  }

  // 左侧菜单默认显示300px
  QTimer::singleShot(0, [this] {
    ui->splitter->setSizes({300, ui->splitter->width() - 300});
  });

  for (auto &item : items) {
    ui->verticalLayout_2->addWidget(item);
    connect(item, &NavMenuItem::clicked, [item, this] {
      if (active_menu_item) {
        active_menu_item->set_active(false);
      }
      item->set_active(true);
      active_menu_item = item;
    });
  }

  show();
}

MainWindow::~MainWindow() {
  delete ui;
}