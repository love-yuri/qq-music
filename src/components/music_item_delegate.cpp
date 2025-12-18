#include "components/music_item_delegate.hpp"
#include "models/music_item_model.hpp"
#include <QPainterPath>
#include <QApplication>

/* 通用样式 */
constexpr auto default_left_padding = 20;                // 默认左侧padding
constexpr auto default_header_left_padding = 60;         // header左侧padding
constexpr auto default_header_tap_size = 3;               // header默认页数
constexpr auto default_header_weight = 10.0 / default_header_tap_size;  // header默认分布

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

/* 背景 */
constexpr auto hover_bg_color = QColor(230, 230, 230); // hover时的背景
constexpr auto hover_bg_radius = 8;                          // hover时的背景radius

/* 品质样式 */
constexpr auto quality_label_height = 20;                     // 品质标签高度
constexpr auto quality_label_width = 36;                      // 品质标签宽度
constexpr auto quality_label_radius = 4;                      // 品质标签圆角
constexpr auto quality_label_font_size = 8;                   // 品质标签大小
constexpr auto quality_hq_label_color = QColor(217, 119, 6);         // 深金色文字
constexpr auto quality_hq_border_color = QColor(217, 119, 6, 100); // 深金色边框
constexpr auto quality_hq_color = QColor(254, 243, 199);             // 浅金色背景
constexpr auto quality_vip_label_color = QColor(255, 255, 255);      // vip标签颜色
constexpr auto quality_vip_bg_color = QColor(238, 130, 238);         // vip标签背景色


struct painter_value {
  // index绘制字体
  QFont index_font = [] {
    QFont font;
    font.setPixelSize(26);
    font.setWeight(QFont::Medium);
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

  const auto is_hover = option.state & QStyle::State_MouseOver;
  if (is_hover) {
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
  const auto title_width = static_cast<int>(pw * default_header_weight);
  const auto title_rect = QRect(img_rect.x() + img_rect.width() + title_left_padding, py, title_width, ph);
  painter->drawText(title_rect, Qt::AlignVCenter | Qt::AlignLeft, music.title);

  // 获取文字的实际边界矩形
  const QFontMetrics fm(painter->font());
  const QRect text_bound = fm.boundingRect(title_rect, Qt::AlignVCenter | Qt::AlignLeft, music.title);

  // 绘制品质标签
  const auto quality_label_rect = QRect(
    text_bound.x() + text_bound.width() + default_left_padding,
    py + (ph - quality_label_height) / 2,
    quality_label_width,
    quality_label_height
  );

  painter->setPen(QPen(quality_hq_border_color));
  painter->setBrush(quality_hq_color);
  painter->drawRoundedRect(quality_label_rect, quality_label_radius, quality_label_radius);

  auto font = painter->font();
  font.setPointSize(quality_label_font_size);
  painter->setFont(font);
  painter->setPen(QPen(quality_hq_label_color));
  painter->drawText(quality_label_rect, Qt::AlignCenter, "无损");

  if (is_hover) {
    // 绘制VIP
    const auto vip_rect = quality_label_rect.translated(quality_label_rect.width() + default_left_padding / 2, 0);
    painter->setPen(Qt::NoPen);
    painter->setBrush(quality_vip_bg_color);
    painter->drawRoundedRect(vip_rect, quality_label_radius, quality_label_radius);
    painter->setPen(QPen(quality_vip_label_color));
    painter->drawText(vip_rect, Qt::AlignCenter, "vip");
  }

  const auto header_tap_width = (pw - default_header_left_padding) / default_header_tap_size;

  // 绘制歌手
  const auto singer_rect_x = default_header_left_padding + header_tap_width;
  const auto singer_rect = QRect(singer_rect_x,py,header_tap_width,ph);
  painter->setFont(painter_value.index_font);
  painter->setPen(QPen(title_text_color));
  painter->drawText(singer_rect, Qt::AlignVCenter | Qt::AlignLeft, "本兮");

  // 绘制专辑
  const auto album_rect_x = default_header_left_padding + header_tap_width * 2;
  const auto album_rect = QRect(album_rect_x,py,header_tap_width,ph);
  painter->drawText(album_rect, Qt::AlignVCenter | Qt::AlignLeft, "一个人的女团");


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
