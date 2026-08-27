#define SERVO_MIN 110  // Pulse untuk 0 derajat
#define SERVO_MAX 500  // Pulse untuk 180 derajat

int angleToPWM(int angle) {
  angle = constrain(angle, 0, 180);
  return map(angle, 0, 180, SERVO_MIN, SERVO_MAX); 
}

void setupMotorConveyor() {
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  ledcAttach(PIN_ENA, 5000, 8); 
}

void setupServos() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);  
  
  // Posisi awal semua servo sortir
  pwm.setPWM(taskBearing.channel, 0, angleToPWM(taskBearing.closedAngle));
  pwm.setPWM(taskNut.channel, 0, angleToPWM(taskNut.closedAngle));
  pwm.setPWM(taskBolt.channel, 0, angleToPWM(taskBolt.closedAngle));
  
  // Posisi awal servo feeder langsung menahan barang (0 derajat)
  pwm.setPWM(FEEDER_CH, 0, angleToPWM(feederClosedAngle));
  
  Serial.println("PCA9685 menyala. Semua servo standby di posisi awal.");
}

void startConveyor() {
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  ledcWrite(PIN_ENA, 150); 
  Serial.println("Konveyor Berjalan.");
}

// Mengatur gerakan halus servo sortir (Ch 0, 1, 2)
void processServoTask(ServoTask &task) {
  unsigned long currentMillis = millis();

  if (task.currentAngle != task.targetAngle) {
    if (currentMillis - task.lastStepTime >= SERVO_STEP_DELAY) {
      if (task.currentAngle < task.targetAngle) task.currentAngle++;
      else task.currentAngle--;
      pwm.setPWM(task.channel, 0, angleToPWM(task.currentAngle));
      task.lastStepTime = currentMillis;
    }
  }
  
  if (task.head != task.tail && !task.isOpen) {
    unsigned long triggerTime = task.triggers[task.head]; 
    if (currentMillis - triggerTime >= task.delayToReach) {
      task.targetAngle = task.openAngle; 
      task.isOpen = true;
      task.openTime = 0; 
      task.head = (task.head + 1) % MAX_QUEUE; 
      Serial.print("Servo channel "); Serial.print(task.channel); Serial.println(" mulai membuka perlahan...");
    }
  }

  if (task.isOpen && task.currentAngle == task.openAngle && task.openTime == 0) {
    task.openTime = currentMillis; 
  }

  if (task.isOpen && task.openTime > 0) {
    if (currentMillis - task.openTime >= SERVO_DURATION) {
      task.targetAngle = task.closedAngle; 
      task.openTime = 0; 
    }
  }

  if (task.isOpen && task.targetAngle == task.closedAngle && task.currentAngle == task.closedAngle) {
    task.isOpen = false; 
  }
}

// ================= LOGIKA INSTAN SERVO FEEDER (CHANNEL 3) =================
void processFeederServo() {
  unsigned long currentMillis = millis();

  // 1. Cek otomatis 5 detik pertama sejak ESP32 ON untuk melepas barang pertama
  if (!initialFeederDone && currentMillis >= 5000) {
    feederShouldOpen = true;
    initialFeederDone = true;
    Serial.println("Feeder: Trigger otomatis 5 detik pertama aktif.");
  }

  // 2. Logika Buka-Tutup Instan (Non-blocking menggunakan millis)
  // A. Servo langsung bergerak ke 90 derajat saat dipicu (Trigger)
  if (feederShouldOpen && !feederIsOpen) {
    pwm.setPWM(FEEDER_CH, 0, angleToPWM(feederOpenAngle)); // Langsung tembak 90 derajat
    feederIsOpen = true;
    feederOpenTime = currentMillis;
    feederShouldOpen = false; 
    Serial.println("Feeder: MEMBUKA Instan (0 -> 90 derajat).");
  }

  // B. Servo langsung bergerak kembali ke 0 derajat setelah 0.50 detik habis
  if (feederIsOpen) {
    if (currentMillis - feederOpenTime >= FEEDER_DURATION) {
      pwm.setPWM(FEEDER_CH, 0, angleToPWM(feederClosedAngle)); // Langsung tembak 0 derajat
      feederIsOpen = false;
      Serial.println("Feeder: MENUTUP Instan (90 -> 0 derajat).");
    }
  }
}