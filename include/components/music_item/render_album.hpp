#pragma once

#include "component_manager.hpp"

namespace music_item_delegate {

/**
 * 绘制专辑列
 */
struct RendererAlbum {
  MusicItemPainterFunc {
    const auto py = rect.y(), pw = rect.width(), ph = rect.height();
    const auto header_tap_width = (pw - default_header_left_padding) / default_header_tap_size;

    // 绘制专辑
    const auto album_rect_x = default_header_left_padding + header_tap_width * 2;
    const auto album_rect = QRect(album_rect_x,py,header_tap_width,ph);
    painter->drawText(album_rect, Qt::AlignVCenter | Qt::AlignLeft, "一个人的女团");
  }
};

}
