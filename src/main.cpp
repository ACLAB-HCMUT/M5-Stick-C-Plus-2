#include <M5StickCPlus2.h>
#include <esp_sleep.h>

#define MOTION_THRESHOLD 0.5f   // G units, tăng để nghiêng nhẹ không wakeup
#define WAKE_DISPLAY_TIME 5000   // 5 giây hiển thị pin
#define SAMPLE_DELAY 75          // 20 ms giữa các sample

void showBattery() {
    StickCP2.Display.clear();
    StickCP2.Display.setCursor(0, 0);

    int bat_mv = StickCP2.Power.getBatteryVoltage();
    StickCP2.Display.printf("Battery:\n\n   %d mV", bat_mv);

    Serial.printf("Battery Voltage: %d mV\n", bat_mv);
}

void goToSleep() {
    Serial.println("Going to IMU motion sleep...");

    StickCP2.Display.clear();
    StickCP2.Display.sleep(); // tắt LCD

    // Light sleep với timer wakeup 20 ms
    esp_sleep_enable_timer_wakeup(SAMPLE_DELAY * 1000); // microseconds
    esp_light_sleep_start();
}

void setup() {
    Serial.begin(115200);
    delay(300);

    auto cfg = M5.config();
    StickCP2.begin(cfg);

    StickCP2.Display.setRotation(1);
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
    StickCP2.Display.setTextColor(GREEN);

    StickCP2.Display.clear();
    StickCP2.Display.println("Initializing IMU...");

    // Init IMU
    if (!StickCP2.Imu.begin()) {
        StickCP2.Display.println("IMU FAILED!");
        while (1) delay(100);
    }

    StickCP2.Display.println("OK!");
    delay(800);

    Serial.println("System Ready. Sleeping...");
    delay(300);

    goToSleep(); // enter first sleep
}

void loop() {
    StickCP2.update();

    // đọc gia tốc
    float ax, ay, az;
    StickCP2.Imu.getAccel(&ax, &ay, &az);

    // loại bỏ trọng lực trục Z
    float axg = ax;
    float ayg = ay;
    float azg = az - 1.0f;

    // magnitude chuẩn
    float magnitude = sqrt(axg*axg + ayg*ayg + azg*azg);

    if (magnitude > MOTION_THRESHOLD) {
        Serial.println("Motion detected! Waking...");

        // Bật LCD
        StickCP2.Display.wakeup();
        StickCP2.Display.setCursor(0, 0);

        // Show battery trong 5 giây
        unsigned long t0 = millis();
        while (millis() - t0 < WAKE_DISPLAY_TIME) {
            showBattery();
            delay(300);
        }

        // Quay lại sleep
        goToSleep();
    } else {
        // Không đủ chuyển động → tiếp tục sleep
        goToSleep();
    }
}
