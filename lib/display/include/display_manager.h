// Copyright (c) 2024 AccessHub
// All rights reserved

#ifndef LIB_DISPLAY_MANAGER_INCLUDE_DISPLAY_MANAGER_H_
#define LIB_DISPLAY_MANAGER_INCLUDE_DISPLAY_MANAGER_H_

#include <string>

#include <LiquidCrystal_I2C.h>

namespace shub {

using Display = LiquidCrystal_I2C;

class DisplayManager {
public:
  DisplayManager(Display &&display);

  virtual ~DisplayManager() = default;

  void TurnOn();

  void ShowCardNumber(std::string const &card_number);

  void ShowInitialScreen();

  void ShowMainScreen();

  void ShowManualInputScreen();

  void ShowCardInputScreen(uint32_t uid);

  void ShowSuccessMessage(const char *message);

  void ShowFailureMessage();

  bool IsOn() const;

private:
  void ShowInfoMessage(char const* message);

private:
  Display display_;
  bool is_on_ = false;
};

}  // namespace shub

#endif  // LIB_DISPLAY_MANAGER_INCLUDE_DISPLAY_MANAGER_H_
