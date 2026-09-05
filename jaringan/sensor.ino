void setupInfrared() {
  pinMode(PIN_IR_NUT, INPUT_PULLUP);
  pinMode(PIN_IR_BOLT, INPUT_PULLUP);
  pinMode(PIN_IR_BEARING, INPUT_PULLUP);
  pinMode(PIN_IR_REJECT, INPUT_PULLUP);
  
  Serial.println("Sensor Infrared (Nut, Bolt, Bearing, Reject) diinisialisasi.");
}

void countItems() {
  // 1. Pembacaan Sensor IR - Nut (Mur)
  int stateNut = digitalRead(PIN_IR_NUT);
  if (stateNut == LOW && lastStateNut == HIGH) {
    Serial.println("\n🔴 [NOTIFIKASI] Sensor IR NUT Terbaca!"); // <-- Notifikasi langsung saat terhalang objek
    countNut++;
    updateLCD();
    feederShouldOpen = true; 
    Serial.print("   -> Total Nut masuk wadah: "); Serial.println(countNut);
  }
  lastStateNut = stateNut;

  // 2. Pembacaan Sensor IR - Bolt (Baut)
  int stateBolt = digitalRead(PIN_IR_BOLT);
  if (stateBolt == LOW && lastStateBolt == HIGH) {
    Serial.println("\n🔵 [NOTIFIKASI] Sensor IR BOLT Terbaca!"); // <-- Notifikasi langsung saat terhalang objek
    countBolt++;
    updateLCD();
    feederShouldOpen = true; 
    Serial.print("   -> Total Bolt masuk wadah: "); Serial.println(countBolt);
  }
  lastStateBolt = stateBolt;

  // 3. Pembacaan Sensor IR - Bearing
  int stateBearing = digitalRead(PIN_IR_BEARING);
  if (stateBearing == LOW && lastStateBearing == HIGH) {
    Serial.println("\n🟢 [NOTIFIKASI] Sensor IR BEARING Terbaca!"); // <-- Notifikasi langsung saat terhalang objek
    countBearing++;
    updateLCD();
    feederShouldOpen = true; 
    Serial.print("   -> Total Bearing masuk wadah: "); Serial.println(countBearing);
  }
  lastStateBearing = stateBearing;

  // 4. Pembacaan Sensor IR - Reject Area
  int stateReject = digitalRead(PIN_IR_REJECT);
  if (stateReject == LOW && lastStateReject == HIGH) {
    Serial.println("\n⚠️ [NOTIFIKASI] Sensor IR REJECT Terbaca!"); // <-- Notifikasi langsung saat terhalang objek
    countReject++;
    updateLCD();
    feederShouldOpen = true; 
    Serial.print("   -> Total Barang Reject: "); Serial.println(countReject);
  }
  lastStateReject = stateReject;
}