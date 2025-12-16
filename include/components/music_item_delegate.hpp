#pragma once

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QMouseEvent>

class MusicItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

public:
  explicit MusicItemDelegate(QObject *parent = nullptr);

  // 绘制
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  // 设置大小
  [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

  // 处理鼠标事件
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:


private:
  /**
   * 绘制背景
   */
  static void drawBackground(QPainter *painter, const QStyleOptionViewItem &option) ;
};
