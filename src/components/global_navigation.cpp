#include "components/global_navigation.hpp"
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QWidget>
#include <unordered_map>

/**
 * 初始化导航
 */
void NavigationManager::init(const QWidget *parent, QWidget *current_widget) {
  std::lock_guard lock(mutex);
  widget_map[parent] = std::make_tuple(nullptr, current_widget, nullptr);
}

/**
 * 获取容器的当前widget
 * @param parent 父容器
 * @return 返回当前的孩子
 */
QWidget *NavigationManager::current_widget(const QWidget *parent) {
  std::lock_guard lock(mutex);
  if (const auto it = widget_map.find(parent); it != widget_map.end()) {
    return std::get<1>(it->second);
  }
  return nullptr;
}

/**
  * 将parent导航到next_parent里
  * @param parent 父容器
  * @param next_widget 下一个页面
  */
void NavigationManager::next(QWidget *parent, QWidget *next_widget) {
  QParallelAnimationGroup *animation_group = nullptr;
  const QWidget *old_widget = nullptr;
  QWidget *current_widget = nullptr;
  {
    std::lock_guard lock(mutex);
    if (const auto iter = widget_map.find(parent); iter != widget_map.end()) {
      old_widget = std::get<0>(iter->second);
      current_widget = std::get<1>(iter->second);
      animation_group = std::get<2>(iter->second);
    }
  }

  // 移除旧动画
  if (animation_group) {
    animation_group->stop();
    delete animation_group;
    delete old_widget;
  }

  const auto contentRect = parent->rect();
  animation_group = new QParallelAnimationGroup(parent);

  // 旧页面：缩小 + 向左滑出 + 淡出（模拟被推到后面）
  if (current_widget) {
    current_widget->setGeometry(contentRect);
    current_widget->raise(); // 确保在下层

    auto *oldEffect = new QGraphicsOpacityEffect(current_widget);
    current_widget->setGraphicsEffect(oldEffect);

    // 缩小并向左移动
    auto *slideOut = new QPropertyAnimation(current_widget, "geometry");
    slideOut->setDuration(500);
    slideOut->setStartValue(contentRect);
    const auto shrinkRect = contentRect.adjusted(
      static_cast<int>(-contentRect.width() * 0.3), // 向左移动
      static_cast<int>(contentRect.height() * 0.1), // 向下缩小
      static_cast<int>(-contentRect.width() * 0.5), // 缩小宽度
      static_cast<int>(-contentRect.height() * 0.1) // 缩小高度
    );
    slideOut->setEndValue(shrinkRect);
    slideOut->setEasingCurve(QEasingCurve::InOutCubic);

    auto *fadeOut = new QPropertyAnimation(oldEffect, "opacity");
    fadeOut->setDuration(500);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::InCubic);

    animation_group->addAnimation(slideOut);
    animation_group->addAnimation(fadeOut);
  }

  // 新页面：从右侧大尺寸弹性滑入 + 淡入
  auto *newEffect = new QGraphicsOpacityEffect(next_widget);
  next_widget->setGraphicsEffect(newEffect);
  next_widget->raise(); // 确保在上层

  // 起始位置：右侧稍微放大
  const auto startRect = QRect(
    contentRect.width() + 50,
    -20,
    static_cast<int>(contentRect.width() * 1.05),
    static_cast<int>(contentRect.height() * 1.05));
  next_widget->setGeometry(startRect);

  auto *slideIn = new QPropertyAnimation(next_widget, "geometry");
  slideIn->setDuration(600);
  slideIn->setStartValue(startRect);
  slideIn->setEndValue(contentRect);
  // 使用OutBack产生弹性效果
  slideIn->setEasingCurve(QEasingCurve::OutBack);

  auto *fadeIn = new QPropertyAnimation(newEffect, "opacity");
  fadeIn->setDuration(400);
  fadeIn->setStartValue(0.0);
  fadeIn->setEndValue(1.0);
  fadeIn->setEasingCurve(QEasingCurve::OutCubic);

  animation_group->addAnimation(slideIn);
  animation_group->addAnimation(fadeIn);

  {
    std::lock_guard lock(mutex);
    widget_map[parent] = std::make_tuple(current_widget, next_widget, animation_group);
  }

  QObject::connect(animation_group, &QAnimationGroup::finished, [=, this] {
    delete current_widget;
    animation_group->deleteLater();
    {
      std::lock_guard lock(mutex);
      widget_map[parent] = std::make_tuple(nullptr, next_widget, nullptr);
    }
  });

  next_widget->show();
  animation_group->start();
}