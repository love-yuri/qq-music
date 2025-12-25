#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QMouseEvent>

namespace music_item_delegate {
class ComponentManager;
}

class MusicItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

  music_item_delegate::ComponentManager * manager;

  int playing_index = -1; // 正在播放的序号

public:
  explicit MusicItemDelegate(music_item_delegate::ComponentManager *manager, QObject *parent = nullptr);

  // 绘制
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  // 设置大小
  [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  // 处理鼠标事件
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};
