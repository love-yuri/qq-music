#include "components/music_item_delegate.hpp"
#include "models/music_item_model.hpp"
#include <QPainterPath>
#include <QApplication>

/* 图片样式 */
constexpr auto music_img_radius = 8;                     // 音乐图片圆角大小
constexpr auto music_img_width = 36;                     // 音乐图片大小
constexpr auto music_img_left_padding = 10;              // 音乐图片距离index padding

/* index序号样式 */
constexpr auto index_2b_width = 40;                      // 2位index宽度
constexpr auto index_3b_width = 60;                      // 3位index宽度
constexpr auto index_left_padding = 20;                  // index左侧padding

/* 标题样式 */
constexpr auto title_text_color = QColor(114, 119, 131); // 标题字体
constexpr auto title_left_padding = 20;                        // 标题左侧 padding
constexpr auto title_widget_weight = 0.3;                      // 标题宽度比重

/* 背景 */
constexpr auto hover_bg_color = QColor(230, 230, 230); // hover时的背景
constexpr auto hover_bg_radius = 8;                         // hover时的背景radius

struct painter_value {
  // index绘制字体
  QFont index_font = [] {
    QFont font;
    font.setPixelSize(26);
    font.setWeight(QFont::Medium);
    font.setFamily("Maple Mono NF CN Medium");
    return font;
  }();
};

/**
 * 获取绘制value
 */
static painter_value& get_painter_value() {
  static painter_value painter_value;
  return painter_value;
}

MusicItemDelegate::MusicItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

void MusicItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
  painter->save();
  painter->setRenderHint(QPainter::Antialiasing);

  if (option.state & QStyle::State_MouseOver) {
    // 绘制 hover 效果
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(hover_bg_color);
    painter->drawRoundedRect(option.rect, hover_bg_radius, hover_bg_radius);
    painter->restore();
  }

  const auto model = reinterpret_cast<const MusicItemModel*>(index.model());
  const MusicItem &music = model->at(index.row());
  const painter_value& painter_value = get_painter_value();
  const auto py = option.rect.y(), pw = option.rect.width(), ph = option.rect.height();

  painter->setFont(painter_value.index_font);
  painter->setPen(QPen(title_text_color));

  // 绘制序号
  const int index_width = index.row() + 1 >= 100 ? index_3b_width : index_2b_width;
  const auto index_rect = QRect(index_left_padding, py, index_width, ph);
  painter->drawText(index_rect, Qt::AlignVCenter | Qt::AlignLeft, QString::number(index.row() + 1).rightJustified(2, '0'));

  // 绘制图片
  const auto img_rect = QRect(index_rect.x() + index_rect.width() + music_img_left_padding, py + (ph - music_img_width) / 2, music_img_width, music_img_width);
  QPainterPath path;
  path.addRoundedRect(img_rect, music_img_radius, music_img_radius);
  painter->save();
  painter->setClipPath(path);
  painter->drawPixmap(img_rect, QPixmap(":/images/test.jpg"));
  painter->restore();

  // 绘制标题
  const auto title_width = pw * title_widget_weight;
  const auto title_rect = QRect(img_rect.x() + img_rect.width() + title_left_padding, py, title_width, ph);
  painter->drawText(title_rect, Qt::AlignVCenter | Qt::AlignLeft, music.title);

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
