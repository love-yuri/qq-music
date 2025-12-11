#include <QApplication>
#include "mainwindow.hpp"

import thread_pool;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  MainWindow window;
  window.show();

  // 初始化进程池
  thread_manager->init();

  return QApplication::exec();
}
