#include "views/my_likes_page.hpp"
#include "components/music_item_delegate.hpp"
#include "components/music_item/render_background.hpp"
#include "models/music_item.hpp"
#include "models/music_item_model.hpp"
#include "ui/ui_my_likes_page.h"
#include <QPainterPath>
#include <QMenu>
#include <QFile>
#include <QPainter>
#include <QTimer>
#include <yuri_log.hpp>

MyLikesPage::MyLikesPage(QWidget *parent): QWidget(parent), ui(new Ui::MyLikesPage) {
  ui->setupUi(this);

  initStyle();
}

QPixmap getRoundRectPixmap(const QPixmap& srcPixMap, const QSize & size, const int radius)
{
  // 获取图片尺寸
  const int imageWidth = size.width();
  const int imageHeight = size.height();

  // 处理大尺寸的图片,保证图片显示区域完整
  const QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

  QPixmap destImage(imageWidth, imageHeight);
  destImage.fill(Qt::transparent);
  QPainter painter(&destImage);
  // 抗锯齿
  painter.setRenderHints(QPainter::Antialiasing, true);
  // 图片平滑处理
  painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
  // 将图片裁剪为圆角
  QPainterPath path;
  const QRect rect(0, 0, imageWidth, imageHeight);
  path.addRoundedRect(rect, radius, radius);
  painter.setClipPath(path);
  painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
  return destImage;
}

void MyLikesPage::initStyle() {
  if (QFile file(":/qss/my_likes_page.qss"); file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    setStyleSheet(styleSheet() + file.readAll());
  }

  ui->verticalLayout_3->setAlignment(Qt::AlignTop);
  ui->horizontalLayout_3->setAlignment(Qt::AlignTop);
  ui->search_edit->addAction(QIcon(":/icons/search.svg"), QLineEdit::ActionPosition::LeadingPosition);

  QTimer::singleShot(0, [this] {
    const auto icon = getRoundRectPixmap(QPixmap(":/images/test.jpg"), ui->play_list_img->size(), 12);
    ui->play_list_img->setPixmap(icon);
  });

  // 创建模型和视图
  const auto *manager = new music_item_delegate::ComponentManager(ui->music_list);
  for (int i = 0; i < 1000; i++) {
    manager->get_model()->addMusic(MusicItem {
      .title = "下雪的季节",
    });
  }

  // 设置视图属性
  auto *listView = ui->music_list;
  listView->setModel(manager->get_model());
  listView->setMouseTracking(true);
  listView->setItemDelegate(manager->get_delegate());
  listView->setSelectionMode(QAbstractItemView::SingleSelection);
  listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);  // 平滑滚动
}