/*
 * @Author: love-yuri yuri2078170658@gmail.com
 * @Date: 2025-11-11 20:08:33
 * @LastEditTime: 2025-11-11 20:32:09
 * @Description:
 */
#pragma once

#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include <QParallelAnimationGroup>
#include "yuri_log.hpp"
#include "components/nav_menu_item.hpp"
#include <QRandomGenerator>
#include "models/nav_types.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow final : public QMainWindow {
  Q_OBJECT
  Ui::MainWindow *ui;
  QWidget *current_widget = nullptr;
  QWidget *last_widget = nullptr;
  NavMenuItem *active_menu_item = nullptr;
  QParallelAnimationGroup *animation_group = nullptr;

public:
  explicit MainWindow(QMainWindow *parent = nullptr);
  ~MainWindow() override;

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  void initStyle();                  // 初始化style
  void initNavMenu();                // 初始化导航菜单
  void addSection(NavSection) const; // 添加一级菜单
  void addPage(NavEntry);            // 添加导航二级菜单
};
