#pragma once

#include "utils/common/qt_macro.hpp"
#include "yuri_log.hpp"
#include <QPropertyAnimation>
#include <QWidget>

class QLabel;

class NavMenuItem final : public QWidget {
  Q_OBJECT
public:
  static constexpr auto widget_radius = 10;                                // 圆角大小
  constexpr static auto active_background = QColor(0xFF, 0xD1, 0xDC, 255); // 活跃时背景色
  constexpr static auto normal_background = QColor(0xFF, 0xD1, 0xDC, 20);  // 默认背景色
  constexpr static auto active_text_color = QColor(238, 10, 36);           // 活跃时字体颜色
  constexpr static auto normal_text_color = QColor(22, 55, 122);           // 默认字体颜色
private:
  bool hovered = false;                  // 是否hover
  bool active = false;                   // 是否active
  QLabel *icon_label = nullptr;          // 图标label
  QPixmap normal_icon;                   // 普通状态图标
  QPixmap active_icon;                   // 活跃状态图标
  QString text;                          // 标签
  QFont font;                            // 标签字体

public:
  NavMenuItem(const QString &icon_name, QString text, QWidget *parent = nullptr);
  void set_active(bool) noexcept;
  QString& get_text() noexcept { return text; }

protected:
  void enterEvent(QEnterEvent *event) override;
  void leaveEvent(QEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void paintEvent(QPaintEvent *event) override;

private:
  void updateIndicator();

  Q_ANIMATION_PROPERTY(QColor, background, normal_background) // 背景色
  Q_ANIMATION_PROPERTY(QColor, color, normal_text_color)      // 字体颜色
  Q_ANIMATION_PROPERTY(int, indicator_height, 0)              // indicator高度

signals:
  void clicked();
};
