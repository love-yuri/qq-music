/*
 * @Author: love-yuri yuri2078170658@gmail.com
 * @Date: 2025-11-18 21:18:55
 * @LastEditTime: 2025-11-18 21:47:26
 * @Description:
 */
module;

#include <rapidjson/document.h>
#include "json_macro.hpp"

export module qq_music_types;

import json;

export namespace qq_music {
struct GetMyFavResult {
  int code;
  int totalNum;

  static GetMyFavResult from_json(const rapidjson::Document& document) {
    GetMyFavResult result;
    JsonSet(code);
    JsonSet(totalNum);
    return result;
  }
};
}; // namespace qq_music