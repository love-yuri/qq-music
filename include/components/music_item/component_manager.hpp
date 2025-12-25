#pragma once

#include <QPainter>
#include "shared_styles.hpp"
#include "components/music_item_delegate.hpp"
#include "models/music_item.hpp"
#include "models/music_item_model.hpp"

#include <QListView>
#include <QRect>

/* 渲染函数 */
#define MusicItemPainterFunc static void painter( \
  music_item_delegate::ComponentManager* manager, \
  QPainter *painter, \
  const QRect &rect, \
  const MusicItem &item, \
  const int index, \
  bool is_hover \
)

/* 鼠标悬浮 */
#define MusicItemMouseOverFunc static void mouse_over( \
  music_item_delegate::ComponentManager* manager, \
  const QRect &rect, \
  MusicItem &item, \
  const int index, \
  const QMouseEvent* event \
)

template <typename T>
concept HasMouseOver = requires(T t, music_item_delegate::ComponentManager* manager, const QRect &rect, MusicItem &music, int row, const QMouseEvent *event) {
  { t.mouse_over(manager, rect, music, row, event) };
};

/* 鼠标点击 */
#define MusicItemMouseClickFunc static void mouse_click( \
  music_item_delegate::ComponentManager* manager, \
  const QRect &rect, \
  MusicItem &item, \
  const int index, \
  const QMouseEvent* event \
)

template <typename T>
concept HasMouseClick = requires(T t, music_item_delegate::ComponentManager* manager, const QRect &rect, MusicItem &music, int row, const QMouseEvent *event) {
  { t.mouse_click(manager, rect, music, row, event) };
};

namespace music_item_delegate {
struct RendererBackground;

/**
 * 组件管理
 */
class ComponentManager: QObject {
  Q_OBJECT

  MusicItemModel* model;
  MusicItemDelegate* delegate;

  QFont index_font = [] {
    QFont font;
    font.setPixelSize(26);
    font.setWeight(QFont::Medium);
    return font;
  }();

  /* friend render */
  friend struct RendererBackground;
  friend struct RendererAlbum;
  friend struct RendererCover;
  friend struct RendererTitle;
  friend struct RendererSinger;
  friend struct RendererIndex;

  /* listView */
  QWidget* widget;

  /* shared config */
  int is_playing_index = -1;

public:
  explicit ComponentManager(QListView *parent = nullptr);

  /**
   * 获取model
   * @return model
   */
  [[nodiscard]] MusicItemModel* get_model() const;

  /**
   * 获取代理
   * @return 代理
   */
  [[nodiscard]] MusicItemDelegate* get_delegate() const;

  /**
   * 界面重绘
   */
  void repaint() const {
    widget->repaint();
  }

  /**
   * 绘制操作
   */
  void painter(QPainter *painter, const QStyleOptionViewItem &option, int row);

  /**
   * 鼠标over操作
   */
  void mouse_over(const QStyleOptionViewItem &option, const QMouseEvent *event, int row);

  /**
   * 点击事件
   */
  void mouser_click(const QStyleOptionViewItem &option, const QMouseEvent *event, int row);
};

}

// #pragma once
//
// #include "component_manager.hpp"
//
// namespace music_item_delegate {
//
// /**
//  * 绘制背景
//  */
// struct RendererTest {
//   MusicItemPainterFunc {
//     const auto py = rect.y(), pw = rect.width(), ph = rect.height();
//
//   }
// };
//
// }