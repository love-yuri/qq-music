#pragma once

#include <QColor>

/**
 * 共享样式
 */

/* index序号样式 */
constexpr auto index_2b_width = 40;                      // 2位index宽度
constexpr auto index_3b_width = 60;                      // 3位index宽度
constexpr auto index_left_padding = 20;                  // index左侧padding

/* 标题样式 */
constexpr auto title_text_color = QColor(114, 119, 131); // 标题字体
constexpr auto title_left_padding = 20;                        // 标题左侧 padding

/* 播放状态样式 */
constexpr auto play_status_width = 36;                   // 播放状态宽度
constexpr auto play_status_color = QColor(170,150,218); // 播放状态背景

/* 通用样式 */
constexpr auto default_left_padding = 20;                // 默认左侧padding
constexpr auto default_header_left_padding = 60;         // header左侧padding
constexpr auto default_header_tap_size = 3;               // header默认页数
constexpr auto default_header_weight = 10.0 / default_header_tap_size;  // header默认分布

/* 背景 */
constexpr auto hover_bg_color = QColor(230, 230, 230); // hover时的背景
constexpr auto hover_bg_radius = 8;                          // hover时的背景radius

/* 品质样式 */
constexpr auto quality_label_height = 20;                     // 品质标签高度
constexpr auto quality_label_width = 36;                      // 品质标签宽度
constexpr auto quality_label_radius = 4;                      // 品质标签圆角
constexpr auto quality_label_font_size = 8;                   // 品质标签大小
constexpr auto quality_hq_label_color = QColor(217, 119, 6);         // 深金色文字
constexpr auto quality_hq_border_color = QColor(217, 119, 6, 100); // 深金色边框
constexpr auto quality_hq_color = QColor(254, 243, 199);             // 浅金色背景
constexpr auto quality_vip_label_color = QColor(255, 255, 255);      // vip标签颜色
constexpr auto quality_vip_bg_color = QColor(238, 130, 238);         // vip标签背景色

/* 专辑图片样式 */
constexpr auto music_cover_radius = 8;                     // 音乐图片圆角大小
constexpr auto music_cover_width = 36;                     // 音乐图片大小
constexpr auto music_cover_left_padding = 10;              // 音乐图片距离index padding
