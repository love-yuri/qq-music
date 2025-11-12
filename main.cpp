/*
 * @Author: love-yuri yuri2078170658@gmail.com
 * @Date: 2025-11-11 20:08:33
 * @LastEditTime: 2025-11-12 18:24:59
 * @Description:
 */
#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow window;
  return a.exec();
}
