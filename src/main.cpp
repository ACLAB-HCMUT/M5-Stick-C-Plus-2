#include <M5StickCPlus2.h>

void setup() {
    Serial.begin(115200);
    delay(500);

    // Init device
    StickCP2.begin();

    StickCP2.Display.setRotation(3);
    StickCP2.Display.setTextSize(2);
    StickCP2.Display.setCursor(0, 0);
    StickCP2.Display.println("IMU TEST");
    StickCP2.Display.println("Init IMU...");
    
    if (!StickCP2.Imu.begin()) {
        Serial.println("IMU init FAILED!");
        StickCP2.Display.println("IMU FAILED!");
        while (1) delay(100);
    }

    StickCP2.Display.println("IMU OK!");
    Serial.println("IMU initialized.");
}

void loop() {
    float ax, ay, az;
    float gx, gy, gz;

    StickCP2.update();  // required by M5Unified

    // Read IMU
    StickCP2.Imu.getAccel(&ax, &ay, &az);
    StickCP2.Imu.getGyro(&gx, &gy, &gz);

    // Print to serial
    Serial.printf("ACC: %.2f %.2f %.2f  |  GYRO: %.2f %.2f %.2f\n",
                  ax, ay, az, gx, gy, gz);

    // Print to screen
    StickCP2.Display.setCursor(0, 40);
    StickCP2.Display.printf("AX: %6.2f\nAY: %6.2f\nAZ: %6.2f\n", ax, ay, az);
    StickCP2.Display.printf("GX: %6.2f\nGY: %6.2f\nGZ: %6.2f\n", gx, gy, gz);

    delay(100);
}
