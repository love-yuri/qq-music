#include "components/nav_menu_item.hpp"
#include <QParallelAnimationGroup>
#include <QHBoxLayout>
#include <QStyle>
#include <QLabel>
#include <QPainter>
#include <QtSvg/QSvgRenderer>
#include <QPropertyAnimation>
#include <utility>

NavMenuItem::NavMenuItem(const QString &icon_name, QString text, QWidget *parent) :
  icon_label(new QLabel(this)),
  normal_icon(QIcon(QString(":/icons/normal/%1").arg(icon_name)).pixmap(32, 32)),
  active_icon(QIcon(QString(":/icons/active/%1").arg(icon_name)).pixmap(32, 32)),
  text(std::move(text))
{
  font.setPixelSize(26);
  font.setWeight(QFont::Medium);
  font.setFamily("Maple Mono NF CN Medium");

  setMaximumHeight(59);

  auto *layout = new QHBoxLayout(this);
  icon_label->setPixmap(normal_icon);
  layout->setContentsMargins(20, 12, 16, 12);
  layout->setSpacing(12);
  layout->addWidget(icon_label);
  layout->addStretch();

  setCursor(Qt::PointingHandCursor);
}

void NavMenuItem::set_active(const bool value) noexcept {
  if (value == active) {
    return;
  }

  active = value;
  hovered = value;
  icon_label->setPixmap(value ? active_icon : normal_icon);
  updateIndicator();
}

void NavMenuItem::enterEvent(QEnterEvent *event) {
  if (active) {
    return;
  }
  hovered = true;
  icon_label->setPixmap(active_icon);
  updateIndicator();
}

void NavMenuItem::leaveEvent(QEvent *event) {
  if (active) {
    return;
  }
  hovered = false;
  icon_label->setPixmap(normal_icon);
  updateIndicator();
}

void NavMenuItem::mouseReleaseEvent(QMouseEvent *event) {
  emit clicked();
}

void NavMenuItem::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

  const auto rect = this->rect();
  const auto label_rect = this->rect().adjusted(65, 0, 0, 0);

  // 绘制圆角背景
  painter.setBrush(m_background);
  painter.setPen(Qt::NoPen);
  painter.drawRoundedRect(rect, widget_radius, widget_radius);

  // 绘制标签
  painter.setFont(font);
  painter.setPen(QPen(m_color));
  painter.drawText(label_rect, Qt::AlignVCenter | Qt::AlignLeft, text);

  // 绘制提示条
  painter.setPen(Qt::NoPen);
  QLinearGradient gradient(10, 0, 14, 0);
  gradient.setColorAt(0, active_text_color);
  gradient.setColorAt(1, QColor(active_text_color.red(),active_text_color.green(), active_text_color.blue(), 180));
  painter.setBrush(gradient);
  const auto indicator_rect = QRect(10, (rect.height() - m_indicator_height) / 2, 4, m_indicator_height);
  painter.drawRoundedRect(indicator_rect, 2, 2);
}

void NavMenuItem::updateIndicator() {
  const auto group = new QParallelAnimationGroup(this);

  auto anim = new QPropertyAnimation(this, backgroundProperty);
  anim->setDuration(400);
  anim->setEasingCurve(QEasingCurve::OutCubic);
  anim->setStartValue(m_background);
  anim->setEndValue(hovered ? active_background : normal_background);
  group->addAnimation(anim);

  anim = new QPropertyAnimation(this, colorProperty);
  anim->setDuration(400);
  anim->setEasingCurve(QEasingCurve::OutCubic);
  anim->setStartValue(m_color);
  anim->setEndValue(hovered ? active_text_color : normal_text_color);
  group->addAnimation(anim);

  anim = new QPropertyAnimation(this, indicator_heightProperty);
  anim->setDuration(400);
  anim->setEasingCurve(QEasingCurve::OutCubic);
  anim->setStartValue(m_indicator_height);
  anim->setEndValue(hovered ? 35 : 0);
  group->addAnimation(anim);

  // 启动动画
  group->start(QAbstractAnimation::DeleteWhenStopped);
}
