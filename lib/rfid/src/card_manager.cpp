// Copyright (c) 2024 AccessHub
// All rights reserved

#include "card_manager.h"

shub::CardManager::CardManager(pin_t ss, pin_t rst) : rfid_(ss, rst) {}

void shub::CardManager::Init() {
    rfid_.PCD_Init();
}

bool shub::CardManager::CheckForCard() {
    if(!rfid_.PICC_IsNewCardPresent() || !rfid_.PICC_ReadCardSerial()) {
        return false;
    }
    
    last_card_data_.size = rfid_.uid.size;
    memcpy(last_card_data_.uid, rfid_.uid.uidByte, rfid_.uid.size);

    rfid_.PICC_HaltA();
    rfid_.PCD_StopCrypto1();

    return true;
}

const shub::CardData& shub::CardManager::GetLastCardData() {
    return last_card_data_;
}

uint32_t shub::CardManager::GetLastCardUid() {
    uint32_t uid = 0;
    for (byte i = 0; i < last_card_data_.size; i++) {
      uid += (last_card_data_.uid[i] << 8*(last_card_data_.size - i - 1));
    }
    return uid;
}
