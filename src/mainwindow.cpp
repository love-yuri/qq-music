#include "mainwindow.hpp"
#include "ui/ui_mainwindow.h"
#include <QMainWindow>
#include <QFile>
#include <QPainter>
#include "yuri_log.hpp"
#include "components/nav_menu_item.hpp"
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QThread>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include "models/nav_types.hpp"

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  initStyle();
  initNavMenu();
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  QTimer::singleShot(0, this, [this] {
    current_widget->setGeometry(ui->right_content->rect());
  });
}

/**
 * 初始化样式
 */
void MainWindow::initStyle() {
  if (QFile file(":/qss/main.qss"); file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    setStyleSheet(styleSheet() + file.readAll());
  }

  // 左侧菜单默认显示300px
  QTimer::singleShot(0, [this] {
    ui->splitter->setSizes({300, ui->splitter->width() - 300});
  });

  resize(700, 450);

  ui->verticalLayout_2->setAlignment(Qt::AlignTop);
}

/**
 * 初始化导航菜单
 */
void MainWindow::initNavMenu() {
  // 初始化菜单
  for (const auto &item : NavItems) {
    std::visit([this]<typename T0>(T0 &&arg) {
      using T = std::decay_t<T0>;
      if constexpr (std::is_same_v<T, NavSection>) {
        addSection(arg);
      } else if constexpr (std::is_same_v<T, NavEntry>) {
        addPage(arg);
      }
    }, item);
  }

  QWidget *content = ui->right_content;
  current_widget = new QPushButton("yuri", content);
  current_widget->setGeometry(content->rect());
}

void MainWindow::addSection(const NavSection type) const {
  switch (type) {
    case NavSection::MyMusic: {
      ui->verticalLayout_2->addWidget(new QLabel("我的音乐"));
      break;
    }
    case NavSection::Online: {
      ui->verticalLayout_2->addWidget(new QLabel("在线音乐"));
      break;
    }
  }
}

void MainWindow::addPage(NavEntry type) {
  NavMenuItem *item;
  switch (type) {
    case NavEntry::Discover: {
      item = new NavMenuItem("play", "发现音乐");
      break;
    }
    case NavEntry::PersonalFm: {
      item = new NavMenuItem("fm", "私人 FM");
      break;
    }
    case NavEntry::Video: {
      item = new NavMenuItem("video", "视频");
      break;
    }
    case NavEntry::Friends: {
      item = new NavMenuItem("friend", "朋友");
      break;
    }
    case NavEntry::Downloads: {
      item = new NavMenuItem("download-manager", "下载管理");
      break;
    }
    case NavEntry::RecentlyPlayed: {
      item = new NavMenuItem("podcast", "最近播放");
      break;
    }
    case NavEntry::LikedSongs: {
      item = new NavMenuItem("my-likes", "我喜欢的音乐");
      break;
    }
    case NavEntry::LocalMusic: {
      item = new NavMenuItem("local-music", "本地音乐");
      break;
    }
    default: {
      yerror << "未知的类型: " << static_cast<int>(type);
      return;
    }
  }

  ui->verticalLayout_2->addWidget(item);
  connect(item, &NavMenuItem::clicked, [this, item] {
    if (active_menu_item == item) {
      return;
    }

    if (active_menu_item) {
      active_menu_item->set_active(false);
    }

    item->set_active(true);
    active_menu_item = item;

    if (animation_group) {
      animation_group->stop();
      delete animation_group;
      delete last_widget;
    }

    last_widget = current_widget;
    current_widget = new QPushButton(item->get_text(), ui->right_content);

    const auto contentRect = ui->right_content->rect();
    current_widget->setGeometry(contentRect);

    // 设置透明度效果
    auto *oldEffect = new QGraphicsOpacityEffect(last_widget);
    last_widget->setGraphicsEffect(oldEffect);

    auto *newEffect = new QGraphicsOpacityEffect(current_widget);
    current_widget->setGraphicsEffect(newEffect);

    // 旧页面：向左滑出 + 淡出
    auto *slideOut = new QPropertyAnimation(last_widget, "geometry");
    slideOut->setDuration(400);
    slideOut->setStartValue(contentRect);
    slideOut->setEndValue(QRect(-contentRect.width(), 0, contentRect.width(), contentRect.height()));
    slideOut->setEasingCurve(QEasingCurve::InOutCubic);

    auto *fadeOut = new QPropertyAnimation(oldEffect, "opacity");
    fadeOut->setDuration(400);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->setEasingCurve(QEasingCurve::InOutQuad);

    // 新页面：从右滑入 + 淡入
    current_widget->setGeometry(QRect(contentRect.width(), 0, contentRect.width(), contentRect.height()));

    auto *slideIn = new QPropertyAnimation(current_widget, "geometry");
    slideIn->setDuration(400);
    slideIn->setStartValue(QRect(contentRect.width(), 0, contentRect.width(), contentRect.height()));
    slideIn->setEndValue(contentRect);
    slideIn->setEasingCurve(QEasingCurve::InOutCubic);

    auto *fadeIn = new QPropertyAnimation(newEffect, "opacity");
    fadeIn->setDuration(400);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->setEasingCurve(QEasingCurve::InOutQuad);

    // 组合所有动画
    animation_group = new QParallelAnimationGroup(this);
    animation_group->addAnimation(slideOut);
    animation_group->addAnimation(fadeOut);
    animation_group->addAnimation(slideIn);
    animation_group->addAnimation(fadeIn);

    connect(animation_group, &QAnimationGroup::finished, [=, this] {
      delete last_widget;
      animation_group->deleteLater();
      animation_group = nullptr;
      last_widget = nullptr;
    });

    current_widget->show();
    animation_group->start();
  });
}