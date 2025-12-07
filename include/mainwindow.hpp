/*
 * @Author: love-yuri yuri2078170658@gmail.com
 * @Date: 2025-11-11 20:08:33
 * @LastEditTime: 2025-11-11 20:32:09
 * @Description:
 */
#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include "yuri_log.hpp"
#include "components/nav_menu_item.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QMainWindow *parent = nullptr);
  ~MainWindow() override;

private:
  Ui::MainWindow *ui;
  NavMenuItem* active_menu_item = nullptr;
  std::vector<NavMenuItem*> items {
    new NavMenuItem("fm", "我的播客"),
    new NavMenuItem("play", "发现音乐"),
    new NavMenuItem("friend", "发现音乐"),
    new NavMenuItem("video", "发现音乐"),
    new NavMenuItem("my-likes", "我的喜欢")
  };
};
