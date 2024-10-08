// Copyright (c) 2024 AccessHub
// All rights reserved

#include "buzzer.h"

#include <Arduino.h>

shub::BuzzerConfig::BuzzerConfig(uint32_t frequency, uint32_t duration) :
      frequency(frequency),
      duration(duration) {}

shub::Buzzer::Buzzer(pin_t buzzer) : 
      buzzer_(buzzer),
      has_config_played_(false) {
  pinMode(buzzer_, Direction::kOutput);
}

void shub::Buzzer::Play(uint32_t frequency, uint32_t duration) const {
  setToneChannel(10);
  tone(buzzer_, frequency, duration);
  // delay(duration);
  // noTone(buzzer_);
}

void shub::Buzzer::Play(BuzzerConfig const& config) const {
  Play(config.frequency, config.duration);
}

void shub::Buzzer::PlayConfig() {
  Play(config_);
  has_config_played_ = true;
}

void shub::Buzzer::LoadConfig(BuzzerConfig &&config) {
  config_ = config;
  has_config_played_ = false;
}

bool shub::Buzzer::HasConfigPlayed() const {
  return has_config_played_;
}
