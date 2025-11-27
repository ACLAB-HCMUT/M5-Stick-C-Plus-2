#include <M5StickCPlus2.h>
#include <esp_sleep.h>

#define MOTION_THRESHOLD 0.750f   // adjust sensitivity (G units)
#define WAKE_DISPLAY_TIME 5000   // 5 seconds

bool woke_from_sleep = false;

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
    StickCP2.Display.sleep();   // turn off LCD

    // Light sleep (CPU pauses but IMU stays running)
    esp_sleep_enable_timer_wakeup(20000);   // wake every 20 ms to sample IMU
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

    goToSleep();   // enter first sleep
}

void loop() {
    StickCP2.update();

    // Read IMU to detect motion after periodic light sleep wakeups
    float ax, ay, az;
    StickCP2.Imu.getAccel(&ax, &ay, &az);

    float magnitude = fabs(ax) + fabs(ay) + fabs(az - 1.0f); // remove gravity

    if (magnitude > MOTION_THRESHOLD) {
        Serial.println("Motion detected! Waking...");

        // Turn display ON
        StickCP2.Display.wakeup();
        StickCP2.Display.setCursor(0, 0);

        // Show battery for 5 seconds
        unsigned long t0 = millis();
        while (millis() - t0 < WAKE_DISPLAY_TIME) {
            showBattery();
            delay(300);
        }

        // go back to sleep
        goToSleep();
    }

    // Not enough movement → go back to sleep again
    goToSleep();
}
