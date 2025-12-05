#include <QApplication>
#include "mainwindow.hpp"
#include "api/play_list.hpp"
import thread_pool;

#include <curl/curl.h>
#include <iostream>

int main(int argc, char *argv[]) {
  curl_global_init(CURL_GLOBAL_DEFAULT);

  QApplication a(argc, argv);

  global_config.qq = "2078170658";
  thread_manager->init();
  thread_manager->addTask([] {
    for (const auto res = qqmusic_api::playlist::get_user_playlists(); const auto &i: res.data.disslist) {
      yuri::Log() << "name: " << i.diss_name << " tid: " << i.tid;
    }
  });

  MainWindow window;
  return QApplication::exec();
}
