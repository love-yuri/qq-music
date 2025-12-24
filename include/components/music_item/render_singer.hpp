#pragma once

#include "component_manager.hpp"

namespace music_item_delegate {

/**
 * 绘制歌手列
 */
struct RendererSinger {
  MusicItemPainterFunc {
    const auto py = rect.y(), pw = rect.width(), ph = rect.height();
    const painter_value& painter_value = get_painter_value();

    const auto header_tap_width = (pw - default_header_left_padding) / default_header_tap_size;

    // 绘制歌手
    const auto singer_rect_x = default_header_left_padding + header_tap_width;
    const auto singer_rect = QRect(singer_rect_x,py,header_tap_width,ph);
    painter->setFont(painter_value.index_font);
    painter->setPen(QPen(title_text_color));
    painter->drawText(singer_rect, Qt::AlignVCenter | Qt::AlignLeft, "本兮");
  }
};

}