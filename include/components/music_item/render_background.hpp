#pragma once

#include "component_manager.hpp"

namespace music_item_delegate {

/**
 * 绘制背景
 */
struct RendererBackground {
  MusicItemPainterFunc {
    if (is_hover) {
      painter->save();
      painter->setPen(Qt::NoPen);
      painter->setBrush(hover_bg_color);
      painter->drawRoundedRect(rect, hover_bg_radius, hover_bg_radius);
      painter->restore();
    }
  }
};

}