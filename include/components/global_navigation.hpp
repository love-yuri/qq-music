#pragma once

#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QWidget>
#include <unordered_map>

class NavigationManager {
  using NavigationItem = std::tuple<QWidget *, QWidget *, QParallelAnimationGroup *>;

  // old_widget, current_widget - animation
  std::unordered_map<const QWidget *, NavigationItem> widget_map{};
  std::mutex mutex;

public:
  NavigationManager() = default;
  virtual ~NavigationManager() = default;

  /**
   * 初始化导航
   */
  void init(const QWidget *parent, QWidget *current_widget);

  /**
   * 获取容器的当前widget
   * @param parent 父容器
   * @return 返回当前的孩子
   */
  QWidget *current_widget(const QWidget *parent);

  /**
  * 将parent导航到next_parent里
  * @param parent 父容器
  * @param next_widget 下一个页面
  */
  void next(QWidget *parent, QWidget *next_widget);
};

// 导出manager
inline auto navigation_manager = std::make_shared<NavigationManager>();