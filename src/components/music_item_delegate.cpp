#include "components/music_item_delegate.hpp"

#include "yuri_log.hpp"
#include "models/music_item_model.hpp"
#include <QPainterPath>
#include <QApplication>

MusicItemDelegate::MusicItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

void MusicItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);

  const auto model = reinterpret_cast<const MusicItemModel*>(index.model());
  const MusicItem &music = model->at(index.row());

  // 绘制背景
  drawBackground(painter, option);

  constexpr static auto normal_text_color = QColor(114, 119, 131);

  QFont font;
  font.setPixelSize(26);
  font.setWeight(QFont::Medium);
  font.setFamily("Maple Mono NF CN Medium");

  painter->setFont(font);
  painter->setPen(QPen(normal_text_color));
  painter->drawText(option.rect, Qt::AlignVCenter | Qt::AlignLeft, music.title);

  painter->restore();
}

QSize MusicItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
  Q_UNUSED(option);
  Q_UNUSED(index);
  return { option.rect.width(), 70 };
}

bool MusicItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) {


  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void MusicItemDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option) {
  const QRect rect = option.rect;

  // painter->fillRect(rect, );

  // 绘制底部分隔线
  painter->setPen(QColor(0xe0e0e0));
  painter->drawLine(rect.bottomLeft(), rect.bottomRight());
}
