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

import global_navigation;

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
    navigation_manager->next(ui->right_content, new QPushButton("yuri", ui->right_content));
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

    navigation_manager->next(ui->right_content, new QPushButton(item->get_text(), ui->right_content));
  });
}