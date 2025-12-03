#include <QApplication>
#include "mainwindow.hpp"
#include "utils/common/log_macros.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow window;
  yuri::Log() << "yuri is yes";
  return QApplication::exec();
}
