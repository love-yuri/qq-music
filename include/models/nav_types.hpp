#pragma once

#include <variant>
#include <array>
/**
 * 导航分组（一级菜单）
 */
enum class NavSection {
  Online,   // 在线音乐
  MyMusic   // 我的音乐
};

/**
 * 导航条目类型（二级菜单）
 */
enum class NavEntry {
  Discover,       // 发现音乐
  PersonalFm,     // 私人 FM
  Video,          // 视频
  Friends,        // 朋友
  Downloads,      // 下载管理
  RecentlyPlayed, // 最近播放
  LikedSongs,     // 我喜欢的音乐
  LocalMusic      // 本地音乐
};

/**
 * 导航菜单列表
 */
using NavItem = std::variant<NavSection, NavEntry>;

/**
 * items
 */
inline constexpr std::array<NavItem, 10> NavItems = {
  NavSection::Online,
  NavEntry::Discover,
  NavEntry::PersonalFm,
  NavEntry::Video,
  NavEntry::Friends,
  NavSection::MyMusic,
  NavEntry::Downloads,
  NavEntry::RecentlyPlayed,
  NavEntry::LikedSongs,
  NavEntry::LocalMusic
};
