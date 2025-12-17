#pragma once

#include <QDateTime>
#include <QString>

// 音乐项数据结构
struct MusicItem {
  long id;                 // 唯一标识
  QString title;           // 标题/歌名
  QString artist;          // 艺术家/歌手
  QString album;           // 专辑名
  QString filePath;        // 文件路径
  QString coverPath;       // 封面图片路径
  int duration;            // 时长(秒)
  QString durationText;    // 时长文本 "03:45"
  qint64 fileSize;         // 文件大小(字节)
  QString format;          // 格式 "mp3", "flac"
};
