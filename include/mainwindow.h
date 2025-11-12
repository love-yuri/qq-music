/*
 * @Author: love-yuri yuri2078170658@gmail.com
 * @Date: 2025-11-11 20:08:33
 * @LastEditTime: 2025-11-11 20:32:09
 * @Description:
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QMainWindow *parent = nullptr);
  virtual ~MainWindow();

private:
  Ui::MainWindow *ui;
};

#endif