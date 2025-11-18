module;

#include <rapidjson/document.h>
#include <stdexcept>
#include <string_view>

export module json;

using namespace rapidjson;

export namespace json {
template <typename T> inline T get(const Value &v) {
  // int
  if constexpr (std::is_same_v<T, int>) {
    if (!v.IsInt())
      throw std::runtime_error("JSON: expected int");
    return v.GetInt();
  }
  // double
  else if constexpr (std::is_same_v<T, double>) {
    if (!v.IsNumber())
      throw std::runtime_error("JSON: expected number");
    return v.GetDouble();
  }
  // bool
  else if constexpr (std::is_same_v<T, bool>) {
    if (!v.IsBool())
      throw std::runtime_error("JSON: expected bool");
    return v.GetBool();
  }
  // string
  else if constexpr (std::is_same_v<T, std::string>) {
    if (!v.IsString())
      throw std::runtime_error("JSON: expected string");
    return std::string(v.GetString());
  }
  // const char*
  else if constexpr (std::is_same_v<T, const char *>) {
    if (!v.IsString())
      throw std::runtime_error("JSON: expected string");
    return v.GetString();
  }
  // 其他类型不支持
  else {
    static_assert(!sizeof(T), "Unsupported type in get<T>()");
  }
}

template <typename T>
inline void set(T &o, const Document &document, std::string_view key) {
  auto itr = document.FindMember(key.data());
  if (itr != document.MemberEnd()) {
    o = get<T>(itr->value);
  }
}

template <typename T>
concept JsonSerializable = requires(const rapidjson::Document &doc) {
  { T::from_json(doc) } -> std::same_as<T>;
};

template <JsonSerializable T> T fromJson(std::string_view str) {
  Document document;
  document.Parse(str.data());
  return T::from_json(document);
}

} // namespace json