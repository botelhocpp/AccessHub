// Copyright (c) 2024 AccessHub
// All rights reserved

#ifndef LIB_KEYPAD_INCLUDE_KEYPAD_H_
#define LIB_KEYPAD_INCLUDE_KEYPAD_H_

#include "matrix_keypad.h"

namespace shub {

enum FunctionKey : char {
  kDone = 'A',
  kRemove = 'B',
  kClear = 'C'
};

class Keypad {
public:
  explicit Keypad(MatrixKeypad const&& keypad);

  virtual ~Keypad() = default;
  
  char CheckPressedButton(uint32_t col) const;

  char CheckPressedButton() const;

  pin_t GetColumm(uint8_t col) const;

public:
  static bool IsKeyValid(char key);

  static bool IsFunctionKey(char key);

public:
  static constexpr char kKeyNotPressed = -1;

private:
  char CheckColumm(uint8_t col) const;

private:
  MatrixKeypad keypad_;
};

}  // namespace shub

#endif  // LIB_KEYPAD_INCLUDE_KEYPAD_H_
