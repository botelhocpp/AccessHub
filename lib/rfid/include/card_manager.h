// Copyright (c) 2024 AccessHub
// All rights reserved

#ifndef LIB_CARD_INCLUDE_CARD_MANAGER_H_
#define LIB_CARD_INCLUDE_CARD_MANAGER_H_

#include <MFRC522.h>

#include "hardware_types.h"

namespace shub {

struct CardData {
    uint8_t uid[10];
    uint8_t size;
};

class CardManager {
    public:
        CardManager(pin_t ss, pin_t rst);
        virtual ~CardManager() = default;

        void Init();

        bool CheckForCard();

        const CardData& GetLastCardData();

        uint32_t GetLastCardUid();

    private:
        MFRC522 rfid_;
        CardData last_card_data_;
};

}  // namespace shub

#endif  // LIB_CARD_INCLUDE_CARD_MANAGER_H_