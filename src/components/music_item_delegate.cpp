#include "components/music_item_delegate.hpp"
#include "components/music_item/render_index.hpp"
#include "components/music_item/component_manager.hpp"
#include "components/music_item/render_album.hpp"
#include "components/music_item/render_background.hpp"
#include "components/music_item/render_cover.hpp"
#include "components/music_item/render_singer.hpp"
#include "components/music_item/render_title.hpp"
#include "models/music_item_model.hpp"
#include <QPainterPath>
#include <QApplication>

using RendererType = std::variant<
  music_item_delegate::RendererBackground,
  music_item_delegate::RendererIndex,
  music_item_delegate::RendererCover,
  music_item_delegate::RendererTitle,
  music_item_delegate::RendererSinger,
  music_item_delegate::RendererAlbum
>;

template<typename Variant>
constexpr auto make_default_variant_array() {
  return []<typename... Types>(std::variant<Types...>*) {
    return std::array{std::variant<Types...>(Types{})...};
  } (static_cast<Variant*>(nullptr));
}

inline constexpr auto renderTypes = make_default_variant_array<RendererType>();

MusicItemDelegate::MusicItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

void MusicItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);

  const auto is_hover = option.state & QStyle::State_MouseOver;
  const auto model = const_cast<MusicItemModel*>(reinterpret_cast<const MusicItemModel*>(index.model()));
  const auto is_playing = index.row() == playing_index;
  MusicItem &music = model->at(index.row());

  const painter_value& painter_value = get_painter_value();
  painter->setFont(painter_value.index_font);

  // 绘制
  for (auto render_type : renderTypes) {
    std::visit([&](const auto &render) {
      render.painter(painter, option.rect, music, index.row(), is_hover, is_playing);
    }, render_type);
  }

  painter->restore();
}

QSize MusicItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_UNUSED(option);
  Q_UNUSED(index);
  return { option.rect.width(), 70 };
}

bool MusicItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
  const int py = option.rect.y();
  const int ph = option.rect.height();
  const int left_padding = index.row() + 1 >= 100 ? 5 : -2;
  const auto y = py + (ph - play_status_width) / 2;
  const auto play_status_rect = QRect(index_left_padding + left_padding, y, play_status_width, play_status_width);

  if (event->type() == QEvent::MouseMove) {
    const auto *mouseEvent = dynamic_cast<QMouseEvent*>(event);
    auto *widget = const_cast<QWidget*>(option.widget);
    if (play_status_rect.contains(mouseEvent->pos())) {
      widget->setCursor(Qt::PointingHandCursor);
    } else {
      widget->setCursor(Qt::ArrowCursor);
    }
  }

  if (event->type() == QEvent::MouseButtonRelease) {
    if (const auto *mouseEvent = dynamic_cast<QMouseEvent *>(event); mouseEvent->button() == Qt::LeftButton) {
      if (play_status_rect.contains(mouseEvent->pos())) {
        playing_index = index.row();
        return true;
      }
    }
  }

  return QStyledItemDelegate::editorEvent(event, model, option, index);
}