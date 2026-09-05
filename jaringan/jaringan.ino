void setupWiFi() {
  Serial.println("Memulai Mode Access Point (Hotspot)...");
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Hotspot ESP32 Aktif! WiFi: "); Serial.println(ssid);
  Serial.print("Alamat IP ESP32: "); Serial.println(IP); 
}

// Fungsi memasukkan barang baru ke antrean memori ESP32
void enqueueTrigger(ServoTask &task, unsigned long time) {
  int nextTail = (task.tail + 1) % MAX_QUEUE;
  if (nextTail != task.head) { // Jika memori antrean tidak penuh
    task.triggers[task.tail] = time;
    task.tail = nextTail;
  } else {
    Serial.println("Peringatan: Antrean penuh! Benda diabaikan.");
  }
}

void handleYoloCommand() {
  if (server.hasArg("item")) {
    String item = server.arg("item");
    unsigned long currentMillis = millis();
    
    if (item == "reject") {
      lastDetection = "Reject";
updateLCD();
      Serial.println("YOLO: Terdeteksi double/reject.");
      server.send(200, "text/plain", "Rejected");
      return; 
    } 
    
 if (item == "nut") {

    lastDetection = "Nut";
    updateLCD();

    enqueueTrigger(taskNut, currentMillis);

    Serial.println("YOLO: Nut terdeteksi, masuk antrean.");
}
 else if (item == "bolt") {

    lastDetection = "Bolt";
    updateLCD();

    enqueueTrigger(taskBolt, currentMillis);

    Serial.println("YOLO: Bolt terdeteksi, masuk antrean.");
}
else if (item == "bearing") {

    lastDetection = "Bearing";
    updateLCD();

    enqueueTrigger(taskBearing, currentMillis);

    Serial.println("YOLO: Bearing terdeteksi, masuk antrean.");
}
    
    server.send(200, "text/plain", "Perintah diterima ESP32");
  } else {
    server.send(400, "text/plain", "Format salah");
  }
}

void setupWebServer() {
  server.on("/detect", HTTP_GET, handleYoloCommand);
  server.begin();
  Serial.println("HTTP Server Menyala.");
}