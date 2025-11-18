/*
 * @Author: love-yuri yuri2078170658@gmail.com
 * @Date: 2025-11-18 18:27:33
 * @LastEditTime: 2025-11-18 19:21:40
 * @Description:
 */
module;

#include "yuri_log.hpp"
#include <curl/curl.h>
#include <memory>
#include <string>
#include <string_view>

export module curl;

export class CurlRequest {
private:
  std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curl{nullptr,
                                                            curl_easy_cleanup};

  std::string response_data{};

  // 回调函数，用于接收数据
  static size_t WriteCallback(char *contents, size_t size, size_t nmemb,
                              CurlRequest *request) {
    size_t total_size = size * nmemb;
    request->response_data.append(contents, total_size);
    return total_size;
  }

  // 初始化操作
  bool init() {
    curl.reset(curl_easy_init());
    if (!curl) {
      yerror << "Failed to initialize curl!";
      return false;
    }

    // 设置回调函数
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION,
                     &CurlRequest::WriteCallback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, this);

    // 设置一些通用选项
    curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L); // 跟随重定向
    curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT, 30L);       // 30秒超时

    return true;
  }

public:
  CurlRequest() = default;

  // 禁止拷贝，允许移动
  CurlRequest(const CurlRequest &) = delete;
  CurlRequest &operator=(const CurlRequest &) = delete;
  CurlRequest(CurlRequest &&) = default;
  CurlRequest &operator=(CurlRequest &&) = default;

  // GET 请求
  static std::string get(std::string_view url) {
    CurlRequest request;
    if (!request.init()) {
      yerror << "curl初始化失败!";
      return {};
    }

    // 设置请求 URL (需要null终止的字符串)
    std::string url_str{url};
    curl_easy_setopt(request.curl.get(), CURLOPT_URL, url_str.c_str());

    // 执行请求
    CURLcode res = curl_easy_perform(request.curl.get());
    if (res != CURLE_OK) {
      yerror << "GET请求失败: " << curl_easy_strerror(res);
      return {};
    }

    return request.response_data;
  }

  // 为后续的 POST 方法预留接口
  static std::string post(std::string_view url, std::string_view data) {
    CurlRequest request;
    auto *curl = request.curl.get();
    if (!request.init()) {
      yerror << "curl初始化失败!";
      return {};
    }

    std::string url_str{url};
    curl_easy_setopt(curl, CURLOPT_URL, url_str.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.data());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      yerror << "POST请求失败: " << curl_easy_strerror(res);
      return {};
    }

    return request.response_data;
  }

  static std::string
  post(std::string_view url, const std::unordered_map<std::string, std::string> &form, const std::unordered_map<std::string, std::string> &headers_map) {
    CurlRequest request;
    if (!request.init()) {
      yerror << "curl初始化失败!";
      return {};
    }

    auto *curl = request.curl.get();

    // ---- 1. 拼接 POST 表单 ----
    std::string post_data;
    for (auto &[k, v] : form) {
      if (!post_data.empty()) {
        post_data += "&";
      }
      post_data += curl_easy_escape(curl, k.c_str(), 0);
      post_data += "=";
      post_data += curl_easy_escape(curl, v.c_str(), 0);
    }

    // ---- 2. 设置 URL ----
    curl_easy_setopt(request.curl.get(), CURLOPT_URL, std::string(url).c_str());

    // ---- 3. 设置 POST 数据 ----
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());

    // ---- 4. 设置 Headers ----
    struct curl_slist *headers = NULL;

    for (auto &[k, v] : headers_map) {
      std::string h = k + ": " + v;
      headers = curl_slist_append(headers, h.c_str());
    }

    // 设置 content-type
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // ---- 5. 执行 ----
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);

    if (res != CURLE_OK) {
      yerror << "POST请求失败: " << curl_easy_strerror(res);
      return {};
    }

    return request.response_data;
  }
};