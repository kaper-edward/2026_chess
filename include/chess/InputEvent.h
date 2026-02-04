#pragma once

#include <string>

struct InputEvent {
  enum class Type {
    Select,
    Command,
    Quit,
    None
  };

  Type type = Type::None;
  std::string payload;
};
