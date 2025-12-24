#pragma once

#include "component_manager.hpp"
#include "yuri_log.hpp"

#include <QPainterPath>

namespace music_item_delegate {

/**
 * 绘制专辑封面
 */
struct RendererCover {
  MusicItemPainterFunc {
    const auto py = rect.y(), ph = rect.height();

    const auto cover_rect = QRect(index_left_padding + 50 + music_cover_left_padding, py + (ph - music_cover_width) / 2, music_cover_width, music_cover_width);
    QPainterPath path;
    path.addRoundedRect(cover_rect, music_cover_radius, music_cover_radius);
    painter->save();
    painter->setClipPath(path);
    painter->drawPixmap(cover_rect, QPixmap(":/images/test.jpg"));
    painter->restore();
  }
};

}