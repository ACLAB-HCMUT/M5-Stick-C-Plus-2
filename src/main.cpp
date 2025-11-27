#include <M5StickCPlus2.h>

void setup() {
    Serial.begin(115200);
    delay(500);

    // Init device
    auto cfg = M5.config();
    StickCP2.begin(cfg);

    StickCP2.Display.setRotation(1);
    StickCP2.Display.setTextSize(1);
    StickCP2.Display.setTextFont(&fonts::Orbitron_Light_24);
    StickCP2.Display.setTextColor(GREEN);
    StickCP2.Display.setTextDatum(top_left);

    StickCP2.Display.clear();
    StickCP2.Display.setCursor(0, 0);
    StickCP2.Display.println("IMU + BAT TEST");
    StickCP2.Display.println("Init IMU...");

    // Init IMU
    if (!StickCP2.Imu.begin()) {
        Serial.println("IMU init FAILED!");
        StickCP2.Display.println("IMU FAILED!");
        while (1) delay(100);
    }

    StickCP2.Display.println("IMU OK!");
    Serial.println("IMU initialized.");
    delay(800);
}

void loop() {
    StickCP2.update();  // required by M5Unified

    float ax, ay, az;
    float gx, gy, gz;

    // Read IMU
    StickCP2.Imu.getAccel(&ax, &ay, &az);
    StickCP2.Imu.getGyro(&gx, &gy, &gz);

    // Read battery
    int bat_mv = StickCP2.Power.getBatteryVoltage();

    // Print to serial monitor
    Serial.printf("BAT: %dmV  |  ACC: %.2f %.2f %.2f  |  GYRO: %.2f %.2f %.2f\n",
                  bat_mv, ax, ay, az, gx, gy, gz);

    // Display on screen
    StickCP2.Display.clear();
    StickCP2.Display.setCursor(0, 0);
    StickCP2.Display.printf("BAT: %d mV\n\n", bat_mv);

    StickCP2.Display.printf("AX: %6.2f\nAY: %6.2f\nAZ: %6.2f\n\n",
                            ax, ay, az);
    StickCP2.Display.printf("GX: %6.2f\nGY: %6.2f\nGZ: %6.2f\n",
                            gx, gy, gz);

    delay(100);
}
