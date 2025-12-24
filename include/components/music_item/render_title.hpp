#pragma once

#include "component_manager.hpp"

namespace music_item_delegate {

/**
 * 绘制歌曲名称
 */
struct RendererTitle {
  MusicItemPainterFunc {
    const auto py = rect.y(), pw = rect.width(), ph = rect.height();

    const auto img_rect = QRect(index_left_padding + 50 + music_cover_left_padding, py + (ph - music_cover_width) / 2, music_cover_width, music_cover_width);

    // 绘制标题
    const auto title_width = static_cast<int>(pw * default_header_weight);
    const auto title_rect = QRect(img_rect.x() + img_rect.width() + title_left_padding, py, title_width, ph);
    painter->drawText(title_rect, Qt::AlignVCenter | Qt::AlignLeft, item.title);

    // 获取文字的实际边界矩形
    const QFontMetrics fm(painter->font());
    const QRect text_bound = fm.boundingRect(title_rect, Qt::AlignVCenter | Qt::AlignLeft, item.title);

    // 绘制品质标签
    const auto quality_label_rect = QRect(
      text_bound.x() + text_bound.width() + default_left_padding,
      py + (ph - quality_label_height) / 2,
      quality_label_width,
      quality_label_height
    );

    painter->setPen(QPen(quality_hq_border_color));
    painter->setBrush(quality_hq_color);
    painter->drawRoundedRect(quality_label_rect, quality_label_radius, quality_label_radius);

    auto font = painter->font();
    font.setPointSize(quality_label_font_size);
    painter->setFont(font);
    painter->setPen(QPen(quality_hq_label_color));
    painter->drawText(quality_label_rect, Qt::AlignCenter, "无损");

    if (is_hover) {
      // 绘制VIP
      const auto vip_rect = quality_label_rect.translated(quality_label_rect.width() + default_left_padding / 2, 0);
      painter->setPen(Qt::NoPen);
      painter->setBrush(quality_vip_bg_color);
      painter->drawRoundedRect(vip_rect, quality_label_radius, quality_label_radius);
      painter->setPen(QPen(quality_vip_label_color));
      painter->drawText(vip_rect, Qt::AlignCenter, "vip");
    }
  }
};

}