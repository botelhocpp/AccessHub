// Copyright (c) 2024 AccessHub
// All rights reserved

#include "display_manager.h"

shub::DisplayManager::DisplayManager(Display &&display) :
      display_(display) {}
  
void shub::DisplayManager::TurnOn() {
  if(!is_on_) {
    display_.init();
    display_.backlight();
    ShowMainScreen();
    is_on_ = true;
  }
}

void shub::DisplayManager::ShowCardNumber(std::string const &card_number) {
  static char buffer[17];
  sprintf(buffer, "%-16s", card_number.c_str());

  display_.clear();
  display_.setCursor(0, 0);
  display_.print(buffer);
}

void shub::DisplayManager::ShowInitialScreen() {
  display_.clear();
  display_.setCursor(3, 0);
  display_.print("AccessHub");
  display_.setCursor(2, 1);
  display_.print("Starting...");
}

void shub::DisplayManager::ShowMainScreen() {
  display_.clear();
  display_.setCursor(3, 0);
  display_.print("AccessHub");
  display_.setCursor(1, 1);
  display_.print("Access Control");
}

void shub::DisplayManager::ShowManualInputScreen() {
  display_.clear();
  display_.setCursor(0, 0);
  display_.print("Password: ");
}

void shub::DisplayManager::ShowCardInputScreen(uint32_t uid) {
  static char buffer[17];
  sprintf(buffer, "%016u", uid);

  display_.clear();
  display_.setCursor(0, 0);
  display_.print(buffer);
}

void shub::DisplayManager::ShowSuccessMessage(const char *message) {
  ShowInfoMessage(message);
}

void shub::DisplayManager::ShowFailureMessage() {
  ShowInfoMessage("Entry Prohibited");
}

bool shub::DisplayManager::IsOn() const {
  return is_on_;
}

void shub::DisplayManager::ShowInfoMessage(char const* message) {
  display_.setCursor(0, 1);
  display_.print(message);
}
