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

using namespace music_item_delegate;


MusicItemDelegate::MusicItemDelegate(ComponentManager *manager, QObject *parent) :
  QStyledItemDelegate(parent), manager(manager) {
}

void MusicItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  painter->setRenderHint(QPainter::Antialiasing);
  manager->painter(painter, option, index.row());
}

QSize MusicItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_UNUSED(option);
  Q_UNUSED(index);
  return { option.rect.width(), 70 };
}

bool MusicItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
  /* 鼠标悬浮 */
  if (event->type() == QEvent::MouseMove) {
    const auto *mouseEvent = static_cast<QMouseEvent*>(event);
    manager->mouse_over(option, mouseEvent, index.row());
  }

  /* 鼠标点击 */
  if (event->type() == QEvent::MouseButtonRelease) {
    if (const auto *mouseEvent = dynamic_cast<QMouseEvent *>(event); mouseEvent->button() == Qt::LeftButton) {
      manager->mouser_click(option, mouseEvent, index.row());
    }
  }

  return QStyledItemDelegate::editorEvent(event, model, option, index);
}