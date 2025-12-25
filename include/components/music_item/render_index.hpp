#pragma once

#include "component_manager.hpp"

#include <yuri_log.hpp>

namespace music_item_delegate {

/**
 * 绘制index 以及index相关东西
 */
struct RendererIndex {
  static QRect indexRect(const QRect &rect, const int index) {
    const auto py = rect.y(), pw = rect.width(), ph = rect.height();

    const int index_width = index + 1 >= 100 ? index_3b_width : index_2b_width;
    return QRect(index_left_padding, py, index_width, ph);
  }

  MusicItemPainterFunc {
    const auto py = rect.y(), pw = rect.width(), ph = rect.height();
    const auto index_rect = indexRect(rect, index);

    if (const auto is_playing = item.is_playing; is_hover || is_playing) {  // hover 或正在播放时显示按钮
      const auto y = py + (ph - play_status_width) / 2;
      const int left_padding = index + 1 >= 100 ? 5 : -2;
      const auto play_status_rect = QRect(index_left_padding + left_padding, y, play_status_width, play_status_width);

      // 绘制圆形背景
      painter->setBrush(play_status_color);
      painter->setPen(Qt::NoPen);
      painter->drawEllipse(play_status_rect);

      painter->setBrush(QBrush(Qt::black));
      painter->setPen(Qt::NoPen);

      const QPointF center = play_status_rect.center();
      constexpr qreal triangle_size = play_status_width * 0.4;

      if (is_playing) {
        // 绘制暂停图标（两条竖线）
        constexpr qreal bar_width = triangle_size * 0.3;
        constexpr qreal bar_height = triangle_size * 1.2;
        constexpr qreal spacing = triangle_size * 0.3;

        const QRectF left_bar(center.x() - spacing / 2 - bar_width,
                        center.y() - bar_height / 2,
                        bar_width, bar_height);
        const QRectF right_bar(center.x() + spacing / 2,
                         center.y() - bar_height / 2,
                         bar_width, bar_height);

        painter->drawRect(left_bar);
        painter->drawRect(right_bar);
      } else {
        // 绘制播放三角形
        QPolygonF triangle;
        triangle << QPointF(center.x() - triangle_size / 3 + 2, center.y() - triangle_size / 2)
                 << QPointF(center.x() - triangle_size / 3 + 2, center.y() + triangle_size / 2)
                 << QPointF(center.x() + triangle_size * 2 / 3, center.y());
        painter->drawPolygon(triangle);
      }

      painter->setPen(QPen(title_text_color));
    } else {
      painter->setPen(QPen(title_text_color));
      painter->drawText(index_rect, Qt::AlignVCenter | Qt::AlignLeft, QString::number(index + 1).rightJustified(2, '0'));
    }
  }

  /**
   * 处理鼠标悬浮
   */
  MusicItemMouseOverFunc {
    if (const auto index_rect = indexRect(rect, index); index_rect.contains(event->pos())) {
      manager->widget->setCursor(Qt::PointingHandCursor);
    } else {
      manager->widget->setCursor(Qt::ArrowCursor);
    }
  }

  /**
   * 处理鼠标点击
   */
  MusicItemMouseClickFunc {
    if (const auto index_rect = indexRect(rect, index); index_rect.contains(event->pos())) {
      item.is_playing = true;
      manager->repaint();
    }
  }
};

}