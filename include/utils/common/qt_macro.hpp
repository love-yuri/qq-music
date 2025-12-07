#pragma once

/**
 * 提供常用qt宏
 */

// 定义一个动画宏
#define Q_ANIMATION_PROPERTY(Type, Name, ...) \
private: \
Type m_##Name {__VA_ARGS__}; \
Q_PROPERTY(Type Name READ Name WRITE set_##Name) \
public: \
static constexpr auto  Name##Property = #Name; \
[[nodiscard]] Type Name() const { \
  return m_##Name; \
} \
void set_##Name(const Type& value) { \
  if (m_##Name != value) { \
    m_##Name = value; \
    update(); \
  } \
}
