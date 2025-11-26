#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("ESP32 Test: Setup complete!");

  int x = 2, y = 3;
  int result = x + y;

  Serial.print("Result of 2 + 3 = ");
  Serial.println(result);
}

void loop() {
  Serial.println("ESP32 running...");
  delay(1000);
}
