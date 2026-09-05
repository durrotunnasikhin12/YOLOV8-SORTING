#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <LiquidCrystal_I2C.h>

// ================= KONFIGURASI JARINGAN (MODE AP) =================
const char* ssid = "ESP32_Sortir_Alat"; 
const char* password = "password123"; 
WebServer server(80);

// ================= KONFIGURASI HARDWARE =================
const int PIN_ENA = 25; 
const int PIN_IN1 = 26;
const int PIN_IN2 = 27;

const int PIN_IR_NUT = 32;
const int PIN_IR_BOLT = 15;
const int PIN_IR_BEARING = 13;
const int PIN_IR_REJECT = 14; 

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
LiquidCrystal_I2C lcd(0x27, 16, 2);

String lastDetection = "-";
// ================= VARIABEL GLOBAL =================
int countNut = 0;
int countBolt = 0;
int countBearing = 0;
int countReject = 0; 

int lastStateNut = HIGH, lastStateBolt = HIGH, lastStateBearing = HIGH;
int lastStateReject = HIGH; 

// Timing Perjalanan Barang Sortir (Dalam milidetik)
const unsigned long TIME_TO_NUT = 1000;     
const unsigned long TIME_TO_BOLT = 2500;    
const unsigned long TIME_TO_BEARING = 2000; 
const unsigned long SERVO_DURATION = 2000;  

// Kecepatan gerakan halus servo SORTIR (Jeda milidetik per 1 derajat)
const unsigned long SERVO_STEP_DELAY = 8; 

// Struktur data servo SORTIR dengan Sistem Antrean & Pergerakan Halus
#define MAX_QUEUE 10
struct ServoTask {
  unsigned long triggers[MAX_QUEUE]; 
  int head; 
  int tail; 
  bool isOpen;               
  unsigned long openTime;    
  unsigned long lastStepTime;
  uint8_t channel;
  unsigned long delayToReach;
  int closedAngle;           
  int openAngle;             
  int currentAngle;          
  int targetAngle;           
};

// Urutan Channel Sortir
ServoTask taskBearing = {{0}, 0, 0, false, 0, 0, 0, TIME_TO_BEARING, 180, 90, 180, 180};   
ServoTask taskNut     = {{0}, 0, 0, false, 0, 0, 2, TIME_TO_NUT, 0, 90, 0, 0};       
ServoTask taskBolt    = {{0}, 0, 0, false, 0, 0, 1, TIME_TO_BOLT, 0, 90, 0, 0};      

// ================= KONFIGURASI SERVO FEEDER (CHANNEL 3) =================
const uint8_t FEEDER_CH = 3;
const int feederClosedAngle = 0;   // Posisi awal menutup (menahan barang)
const int feederOpenAngle = 30;    // Posisi membuka (melepas barang)
const unsigned long FEEDER_DURATION = 100; // Membuka selama 0.50 detik (500 ms)

bool feederIsOpen = false;
unsigned long feederOpenTime = 0;
bool initialFeederDone = false; 
bool feederShouldOpen = false;  
void setupLCD();
void updateLCD();
#endif