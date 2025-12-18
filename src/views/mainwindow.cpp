#include "views/mainwindow.hpp"
#include "ui/ui_mainwindow.h"
#include <QMainWindow>
#include <QFile>
#include <QPainter>
#include "yuri_log.hpp"
#include "components/nav_menu_item.hpp"
#include <QTimer>
#include <QThread>
#include "models/nav_types.hpp"
#include "components/global_navigation.hpp"
#include "views/my_likes_page.hpp"

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
    const auto current = navigation_manager->current_widget(ui->right_content);
    current->setGeometry(ui->right_content->rect());
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

  constexpr auto radio = 1;
  resize(1920 * radio, 1080 * radio);

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

  navigation_manager->init(ui->right_content, new MyLikesPage(ui->right_content));
}

void MainWindow::addSection(const NavSection type) const {
  QLabel *label = nullptr;
  switch (type) {
    case NavSection::MyMusic: {
      label = new QLabel("我的音乐");
      break;
    }
    case NavSection::Online: {
      label =new QLabel("在线音乐");
      break;
    }
    default: throw std::runtime_error("暂未支持的标签!");
  }

  QFont font;
  font.setPointSize(16);
  label->setFont(font);
  label->setStyleSheet(R"(
    min-height: 40px;
    color: rgba(108, 105, 105, 0.85);
    font-weight: 600;
    margin-bottom: -4;
  )");
  ui->verticalLayout_2->addWidget(label);
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

    active_menu_item = item;
    active_menu_item->set_active(true);

    navigation_manager->next(ui->right_content, new QPushButton(item->get_text(), ui->right_content));
  });
}