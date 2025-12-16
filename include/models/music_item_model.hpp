#pragma once

#include "music_item.hpp"
#include <QStandardItemModel>

class MusicItemModel : public QAbstractListModel {
  Q_OBJECT
  QList<MusicItem> music_list;

public:
  enum MusicRole {
    TitleRole = Qt::UserRole + 1, // 歌名
    ArtistRole,                   // 歌手
    AlbumRole,                    // 专辑
    FilePathRole,                 // 文件路径
    CoverPathRole,                // 封面路径
    DurationRole,                 // 时长(秒)
    DurationTextRole,             // 时长文本
    FileSizeRole,                 // 文件大小
    FormatRole,                   // 格式
    BitrateRole,                  // 比特率
    SampleRateRole,               // 采样率
    AddTimeRole,                  // 添加时间
    PlayCountRole,                // 播放次数
    GenreRole,                    // 流派
    YearRole,                     // 年份
    LyricsRole,                   // 歌词
    IsFavoriteRole                // 是否收藏
  };

  explicit MusicItemModel(QObject *parent = nullptr): QAbstractListModel(parent) {

  }

  // 添加音乐
  // 直接存储 MusicItem 列表
  void addMusic(const MusicItem &music) {
    beginInsertRows(QModelIndex(), static_cast<int>(music_list.size()), static_cast<int>(music_list.size()));
    music_list.append(music);
    endInsertRows();
  }

  [[nodiscard]] int rowCount(const QModelIndex &parent) const override {
    return static_cast<int>(music_list.size());
  }

  [[nodiscard]] QVariant data(const QModelIndex &index, const int role) const override {
    if (!index.isValid() || index.row() >= music_list.size()) {
      return {};
    }

    const MusicItem &music = music_list.at(index.row());
    switch (role) {
      case Qt::DisplayRole:
      case TitleRole: return music.title;
      case ArtistRole: return music.artist;
      case AlbumRole: return music.album;
      case FilePathRole: return music.filePath;
      case CoverPathRole: return music.coverPath;
      case DurationRole: return music.duration;
      case FileSizeRole: return music.fileSize;
      case FormatRole: return music.format;
      default: return {};
    }
  }

  // 获取音乐信息
  [[nodiscard]] MusicItem& at(const int row) {
    return music_list[row];
  }

  // 获取音乐信息
  [[nodiscard]] const MusicItem &at(const int row) const {
    return music_list[row];
  }
};