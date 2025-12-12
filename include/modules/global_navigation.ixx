module;
#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

#include "yuri_log.hpp"
#include <QWidget>
#include <unordered_map>
export module global_navigation;

// old_widget, current_widget - animation
using NavigationItem = std::tuple<QWidget *, QWidget*, QParallelAnimationGroup *>;

class NavigationManager {
  std::unordered_map<QWidget *, NavigationItem> widget_map{};
  std::mutex mutex;

public:
  NavigationManager() = default;
  virtual ~NavigationManager() = default;

  /**
   * 初始化导航
   */
  void init(QWidget *parent, QWidget *current_widget) {
    std::lock_guard lock(mutex);
    widget_map[parent] = std::make_tuple(nullptr, current_widget, nullptr);
  }

  /**
   * 获取容器的当前widget
   * @param parent 父容器
   * @return 返回当前的孩子
   */
  QWidget* current_widget(QWidget* parent) {
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
  void next(QWidget *parent, QWidget *next_widget) {
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

    // 组合所有动画
    animation_group = new QParallelAnimationGroup(parent);

    // 旧页面：向左滑出 + 淡出
    if (current_widget) {
      current_widget->setGeometry(contentRect);

      // 设置透明度效果
      auto *oldEffect = new QGraphicsOpacityEffect(current_widget);
      current_widget->setGraphicsEffect(oldEffect);

      auto *slideOut = new QPropertyAnimation(current_widget, "geometry");
      slideOut->setDuration(400);
      slideOut->setStartValue(contentRect);
      slideOut->setEndValue(QRect(-contentRect.width(), 0, contentRect.width(), contentRect.height()));
      slideOut->setEasingCurve(QEasingCurve::InOutCubic);

      auto *fadeOut = new QPropertyAnimation(oldEffect, "opacity");
      fadeOut->setDuration(400);
      fadeOut->setStartValue(1.0);
      fadeOut->setEndValue(0.0);
      fadeOut->setEasingCurve(QEasingCurve::InOutQuad);

      animation_group->addAnimation(slideOut);
      animation_group->addAnimation(fadeOut);
    }

    // 新页面：从右滑入 + 淡入
    auto *newEffect = new QGraphicsOpacityEffect(next_widget);
    next_widget->setGraphicsEffect(newEffect);
    next_widget->setGeometry(QRect(contentRect.width(), 0, contentRect.width(), contentRect.height()));

    auto *slideIn = new QPropertyAnimation(next_widget, "geometry");
    slideIn->setDuration(400);
    slideIn->setStartValue(QRect(contentRect.width(), 0, contentRect.width(), contentRect.height()));
    slideIn->setEndValue(contentRect);
    slideIn->setEasingCurve(QEasingCurve::InOutCubic);

    auto *fadeIn = new QPropertyAnimation(newEffect, "opacity");
    fadeIn->setDuration(400);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);

    animation_group->addAnimation(slideIn);
    animation_group->addAnimation(fadeIn);

    {
      std::lock_guard lock(mutex);
      widget_map[parent] = std::make_tuple(current_widget, next_widget, animation_group);
    }

    animation_group->connect(animation_group, &QAnimationGroup::finished, [=, this] {
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
};

// 导出manager
export auto navigation_manager = std::make_shared<NavigationManager>();