#pragma once
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MyLikesPage;
}
QT_END_NAMESPACE

class MyLikesPage final : public QWidget {
  Q_OBJECT
  Ui::MyLikesPage* ui;

public:
  explicit MyLikesPage(QWidget* parent = nullptr);

private:
  void initStyle();
};
