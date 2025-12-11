/*
 * @Author: love-yuri yuri2078170658@gmail.com
 * @Date: 2025-11-11 20:08:33
 * @LastEditTime: 2025-11-11 20:32:09
 * @Description:
 */
#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMouseEvent>
#include <QLabel>
#include <QParallelAnimationGroup>
#include "yuri_log.hpp"
#include "components/nav_menu_item.hpp"
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class TestWidget : public QWidget {
  Q_OBJECT
  QString text;
public:
  explicit TestWidget(QString text, QWidget *parent = nullptr): QWidget(parent), text(std::move(text)) {
    const auto layout = new QVBoxLayout(this);
    const auto label = new QLabel();
    label->setText(this->text);
    label->setMinimumHeight(40);
    label->setFont(QFont("Maple Mono NF CN Medium", 26, QFont::Bold));
    layout->addWidget(label);
    const QString randomColor = QString("#%1").arg(QRandomGenerator::global()->generate() & 0xFFFFFF, 6, 16, QLatin1Char('0'));
    setStyleSheet(QString(R"(
      QWidget {
        background: %1;
        color: #000;
      }
    )").arg(randomColor));

    setLayout(layout);
    show();
  }
};

class MainWindow final : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QMainWindow *parent = nullptr);
  ~MainWindow() override;

protected:
  void resizeEvent(QResizeEvent *event) override;

private:
  Ui::MainWindow *ui;
  QWidget* current_widget = nullptr;
  QWidget* last_widget = nullptr;
  NavMenuItem* active_menu_item = nullptr;
  QParallelAnimationGroup *animation_group = nullptr;
  std::vector<NavMenuItem*> items {
    new NavMenuItem("fm", "我的播客"),
    new NavMenuItem("play", "发现音乐"),
    new NavMenuItem("friend", "发现音乐"),
    new NavMenuItem("video", "发现音乐"),
    new NavMenuItem("my-likes", "我的喜欢")
  };
};
