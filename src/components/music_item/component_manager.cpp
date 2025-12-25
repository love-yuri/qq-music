#include "components/music_item/component_manager.hpp"
#include "components/music_item/render_index.hpp"
#include "components/music_item/render_album.hpp"
#include "components/music_item/render_cover.hpp"
#include "components/music_item/render_title.hpp"
#include "components/music_item/render_singer.hpp"
#include "components/music_item/render_background.hpp"

using namespace music_item_delegate;

using RendererType = std::variant<
  RendererBackground,
  RendererIndex,
  RendererCover,
  RendererTitle,
  RendererSinger,
  RendererAlbum
>;

template<typename Variant>
constexpr auto make_default_variant_array() {
  return []<typename... Types>(std::variant<Types...>*) {
    return std::array{std::variant<Types...>(Types{})...};
  } (static_cast<Variant*>(nullptr));
}

inline constexpr auto renderTypes = make_default_variant_array<RendererType>();

ComponentManager::ComponentManager(QListView *parent) :
  model(new MusicItemModel(parent)),
  delegate(new MusicItemDelegate(this, parent)),
  widget(parent) {

}

MusicItemModel *ComponentManager::get_model() const {
  return model;
}

MusicItemDelegate *ComponentManager::get_delegate() const {
  return delegate;
}

void ComponentManager::painter(QPainter *painter, const QStyleOptionViewItem &option, int row) {
  const auto is_hover = option.state & QStyle::State_MouseOver;
  MusicItem &music = model->at(row);

  // 预制字体
  painter->setFont(index_font);

  // 绘制
  for (auto render_type : renderTypes) {
    std::visit([&](const auto &render) {
      render.painter(this, painter, option.rect, music, row, is_hover);
    }, render_type);
  }
}

void ComponentManager::mouse_over(const QStyleOptionViewItem &option, const QMouseEvent *event, int row) {
  auto &music = model->at(row);
  for (auto render_type : renderTypes) {
    std::visit([&]<typename T0>(const T0 &render) {
      if constexpr (HasMouseOver<T0>) {
        render.mouse_over(this, option.rect, music, row, event);
      }
    }, render_type);
  }
}

void ComponentManager::mouser_click(const QStyleOptionViewItem &option, const QMouseEvent *event, int row) {
  auto &music = model->at(row);
  for (auto render_type : renderTypes) {
    std::visit([&]<typename T0>(const T0 &render) {
      if constexpr (HasMouseClick<T0>) {
        render.mouse_click(this, option.rect, music, row, event);
      }
    }, render_type);
  }
}