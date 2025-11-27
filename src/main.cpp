/*
 * SPDX-FileCopyrightText: 2025 M5Stack
 * SPDX-License-Identifier: MIT
 */

#include "M5StickCPlus2.h"

void showText(const char* text) {
    StickCP2.Display.clear();
    StickCP2.Display.drawString(text,
                                StickCP2.Display.width() / 2,
                                StickCP2.Display.height() / 2);
}

void beep() {
    StickCP2.Speaker.tone(8000, 40);
}

void setup() {
    auto cfg = M5.config();
    StickCP2.begin(cfg);

    StickCP2.Display.setRotation(1);
    StickCP2.Display.setTextColor(GREEN);
    StickCP2.Display.setTextDatum(middle_center);
    StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
    StickCP2.Display.setTextSize(1);

    showText("Button Test");
}

void loop() {
    StickCP2.update();

    // ----- BTN A -----
    if (StickCP2.BtnA.wasPressed()) {
        beep();
        showText("A Pressed");
    }
    if (StickCP2.BtnA.wasReleased()) {
        beep();
        showText("A Released");
    }

    // ----- BTN B -----
    if (StickCP2.BtnB.wasPressed()) {
        beep();
        showText("B Pressed");
    }
    if (StickCP2.BtnB.wasReleased()) {
        beep();
        showText("B Released");
    }

    // ----- PWR BUTTON -----
    if (StickCP2.BtnPWR.wasClicked()) {
        beep();
        showText("PWR Click");
    }
    if (StickCP2.BtnPWR.wasHold()) {
        beep();
        showText("PWR Hold");
    }
}