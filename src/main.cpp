// Copyright (c) 2024 AccessHub
// All rights reserved

#include <queue>

#include <SPI.h>
#include <Arduino.h>

#include "card_manager.h"
#include "protocol_manager.h"
#include "buzzer_manager.h"
#include "display_manager.h"
#include "servo_manager.h"
#include "keypad.h"

#define NETWORK_SSID  "BOLETO"
#define NETWORK_PASS  "boleto123"
#define BROKER_IP     "10.42.0.1"

shub::Keypad keypad(shub::MatrixKeypad({13, 12, 14, 27}, {26, 25, 33, 32}));
shub::BuzzerManager buzzer_manager(shub::Buzzer(4));
shub::DisplayManager display_manager(shub::Display(0x27, 16, 2));
shub::ProtocolManager protocol_manager(NETWORK_SSID, NETWORK_PASS);
shub::CardManager card_manager(5, 16);
shub::ServoManager servo_manager(17);

std::string user_input;
bool user_input_updated = false;

struct access_t {
  std::string name;
  bool input_finished;
  bool had_response;
  bool has_access;
};

access_t user_access = {
  .name = "",
  .input_finished = false,
  .had_response = false,
  .has_access = false
};

void KeypadHandleInput(uint8_t col);

void ProtocolCallback(char* topic, uint8_t* message, unsigned int length);

void setup() {
  display_manager.TurnOn();
  display_manager.ShowInitialScreen();

  Serial.begin(115200);
  Serial.println("Terminal OK!");
  
  SPI.begin();
  card_manager.Init();

  protocol_manager.ConnectToWifi();
  protocol_manager.ConnectToMqtt(BROKER_IP, 1883, "mybroker", "12345", "ESP32-SHUB");
  protocol_manager.SubscribeTopic("info/access");
  protocol_manager.SetCallback(&ProtocolCallback);

  attachInterrupt(digitalPinToInterrupt(keypad.GetColumm(0)), []() {
    KeypadHandleInput(0);
  }, FALLING);

  attachInterrupt(digitalPinToInterrupt(keypad.GetColumm(1)), []() {
    KeypadHandleInput(1);
  }, FALLING);

  attachInterrupt(digitalPinToInterrupt(keypad.GetColumm(2)), []() {
    KeypadHandleInput(2);
  }, FALLING);

  attachInterrupt(digitalPinToInterrupt(keypad.GetColumm(3)), []() {
    KeypadHandleInput(3);
  }, FALLING);
  
  display_manager.ShowMainScreen();
}

void loop() {
  if(card_manager.CheckForCard()) {
    uint32_t uid = card_manager.GetLastCardUid();

    Serial.println(uid);

    protocol_manager.PublishMessage("info/user", protocol_manager.SerializeJson("user", uid));
    buzzer_manager.LoadInputSequence();
    display_manager.ShowCardInputScreen(uid);

    user_access.input_finished = true;
  }
  else if(user_input_updated) {

    user_input_updated = false;

    if(user_input.back() >= 'A') {
      if(user_input.back() == shub::FunctionKey::kClear) {
        user_input.clear();
        display_manager.ShowMainScreen();
        return;
      }
      else if(user_input.back() == shub::FunctionKey::kRemove) {
        user_input.pop_back();
      }
      else if(user_input.back() == shub::FunctionKey::kDone) {
        user_access.input_finished = true;
        user_input.pop_back();
        uint32_t uid = (uint32_t) std::atoll(user_input.c_str());
        Serial.println(uid);
        protocol_manager.PublishMessage("info/user", protocol_manager.SerializeJson("user", uid));
        user_input.clear();
        return;
      }
      
      user_input.pop_back();
    }
    display_manager.ShowCardNumber(user_input);
  }

  buzzer_manager.PlayLoadedSequence();

  if(user_access.input_finished && user_access.had_response) {
    if(user_access.has_access) {
      char buffer[17];
      snprintf(buffer, 17, "%-16s", user_access.name.c_str());
      display_manager.ShowSuccessMessage(buffer);
      buzzer_manager.PlaySuccessSequence();
      servo_manager.SetAngle(180);
    }
    else {
      display_manager.ShowFailureMessage();
      buzzer_manager.PlayFailureSequence();
      servo_manager.SetAngle(0);
    }

    user_access.input_finished = false;
    user_access.had_response = false;
    user_access.has_access = false;
    
    delay(3000);

    display_manager.ShowMainScreen();
  }

  protocol_manager.VerifyWifiConnection();
  
  protocol_manager.ClientLoop();
}

void ProtocolCallback(char* topic, uint8_t* message, unsigned int length) {
  Serial.print(topic);
  Serial.print(": ");
  for(int i = 0; i < length; i++) {
    Serial.print((char) message[i]);
  }
  Serial.println();

  if(user_access.input_finished && strcmp(topic, "info/access") == 0) {
    user_access.had_response = true;
    if(strncmp((char *) message, "nok", length) == 0) {
      user_access.has_access = false;
    }
    else {
      user_access.name = std::string((char *) message, length);
      user_access.has_access = true;
    }
  }
}

void KeypadHandleInput(uint8_t col) {
  char pressed_key = keypad.CheckPressedButton(col);
  if(shub::Keypad::IsKeyValid(pressed_key)) {
    Serial.print(pressed_key);
    if(pressed_key != shub::FunctionKey::kDone) {
      buzzer_manager.LoadInputSequence();
    }
    user_input += pressed_key;
    user_input_updated = true;
  }
}
