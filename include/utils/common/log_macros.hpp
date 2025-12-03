#pragma once

import yuri_log;

/**
 * 日志宏
 */

#ifndef yinfo
#define yinfo ::yuri::Log(__func__, __LINE__, ::yuri::Log::LogLevel::Info)
#endif

#ifndef yerror
#define yerror ::yuri::Log(__func__, __LINE__, ::yuri::Log::LogLevel::Error)
#endif

#ifndef ywarn
#define ywarn ::yuri::Log(__func__, __LINE__, ::yuri::Log::LogLevel::Warning)
#endif

#ifndef ydebug
#define ydebug ::yuri::Log(__func__, __LINE__, ::yuri::Log::LogLevel::Debug)
#endif