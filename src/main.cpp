#include <QApplication>
#include "views/mainwindow.hpp"

import thread_pool;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QApplication::setFont(QFont("Maple Mono NF CN Medium"));
  MainWindow window;
  window.show();

  return QApplication::exec();
}
