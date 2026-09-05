#include "config.h"

void setup() {

  Serial.begin(115200);

  setupWiFi();

  setupMotorConveyor();

  setupInfrared();

  setupServos();

  setupLCD();

  setupWebServer();

  startConveyor();

}
void loop() {
  // 1. Dengarkan perintah HTTP dari YOLO
  server.handleClient();
  
  // 2. Proses pergerakan servo sortir (Non-blocking - Gerakan Halus)
  processServoTask(taskNut);
  processServoTask(taskBolt);
  processServoTask(taskBearing);
  
  // 3. Proses pergerakan servo feeder (Non-blocking - Gerakan Instan 0.50 Detik)
  processFeederServo(); 
  
  // 4. Proses penghitungan barang & trigger feeder kembali
  countItems();
}