🛑🛑🛑🛑🛑🛑🛑🛑🛑 code🛑🛑🛑🛑🛑🛑


#include <PZEM004Tv30.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED SETTINGS
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// HARDWARE PINS
#define RELAY_PIN 5

// ENERGY THRESHOLD in Wh
#define ENERGY_THRESHOLD 5.0 

// PZEM SENSOR SETTINGS
PZEM004Tv30 pzem(Serial2, 16, 17); // RX=16, TX=17

// STABILIZATION DELAY (ms)
const unsigned long STARTUP_DELAY = 10000; // 10 seconds

// STATE TRACKERS
unsigned long startupTime = 0;
bool startupComplete = false;
unsigned long lastCalcMillis = 0;
bool energyCalculationStarted = false;

// ENERGY TRACKING
float accumulatedEnergy = 0.0;

// RELAY STATE
bool relayState = true;

void setup() {
  Serial.begin(115200);

  // Initialize RELAY (active LOW: LOW=ON, HIGH=OFF)
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); 

  // OLED INIT
  Wire.begin(21, 22); // SDA=21, SCL=22
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("OLED allocation failed"));
    for (;;);
  }
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Show start message
  display.clearDisplay();
  display.setCursor(10, 20);
  display.println("Smart Plug System");
  display.setCursor(15, 40);
  display.println("Initializing...");
  display.display();
  delay(2000);

  // Reset PZEM energy register (hardware level - optional)
  pzem.resetEnergy();
  delay(1000);

  // Manual software energy reset
  accumulatedEnergy = 0.0;
  energyCalculationStarted = false;

  Serial.println("Energy counter RESET");

  // Set stabilization wait start
  startupTime = millis();
  Serial.println("Sensor stabilization starting (10s wait).");

  // OLED Stabilization notice
  display.clearDisplay();
  display.setCursor(10, 30);
  display.println("Stabilizing...");
  display.display();
}

void loop() {
  unsigned long currentMillis = millis();

  // Read sensor values
  float voltage = pzem.voltage();
  float current  = pzem.current();
  float power    = pzem.power();
  float pf       = pzem.pf();

  // --- STABILIZATION PERIOD ---
  if (!startupComplete) {
    unsigned long elapsed = currentMillis - startupTime;
    if (elapsed < STARTUP_DELAY) {
      int secondsLeft = (STARTUP_DELAY - elapsed) / 1000;
      Serial.print("Stabilizing: "); Serial.print(secondsLeft); Serial.println("s remaining.");

      // Show countdown
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(10, 20);
      display.println("Stabilizing...");
      display.setCursor(35, 40);
      display.print(secondsLeft); display.println(" sec left");
      display.display();
      delay(1000);
      return; // Do NOT accumulate energy during stabilization
    } else {
      // End of stabilization - ALL ENERGY RESET
      startupComplete = true;
      accumulatedEnergy = 0.0;
      lastCalcMillis = currentMillis;
      energyCalculationStarted = true;

      Serial.println("\n=== Stabilization Done ===");
      Serial.println("Energy RESET. Starting calculation...");
      display.clearDisplay();
      display.setCursor(10, 20);
      display.println("Stabilization Done");
      display.setCursor(10, 40);
      display.println("Energy: 0 Wh");
      display.display();
      delay(2000);
    }
  }

  // --- ENERGY CALCULATION (AFTER STABILIZATION ONLY) ---
  if (energyCalculationStarted && !isnan(power) && power > 0) {
    float timeElapsedHr = (currentMillis - lastCalcMillis) / 3600000.0; 
    float energyInc = power * timeElapsedHr;
    accumulatedEnergy += energyInc;
    lastCalcMillis = currentMillis;
  } else if (energyCalculationStarted) {
    // Even if power is 0, keep lastCalcMillis updated
    lastCalcMillis = currentMillis;
  }

  // --- OLED Display ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Smart Plug Monitor");
  display.setCursor(0, 15);
  display.print("V: "); display.print(voltage, 1); display.println(" V");
  display.setCursor(0, 25);
  display.print("I: "); display.print(current, 2); display.println(" A");
  display.setCursor(0, 35);
  display.print("P: "); display.print(power, 1); display.println(" W");
  display.setCursor(0, 45);
  display.print("E: "); display.print(accumulatedEnergy, 3); display.println(" Wh");
  
  // --- THRESHOLD DETECTION AND ACTION ---
  if (accumulatedEnergy >= ENERGY_THRESHOLD) {
    // Switch the relay OFF (active LOW relay), energy reset
    digitalWrite(RELAY_PIN, HIGH);
    relayState = false;
    Serial.println("\n*** ENERGY THRESHOLD EXCEEDED ***");
    Serial.print("Energy: "); Serial.print(accumulatedEnergy, 4); Serial.print(" Wh (Limit: "); Serial.print(ENERGY_THRESHOLD, 1); Serial.println(" Wh)");
    
    // Energy value RESET
    accumulatedEnergy = 0.0;
    lastCalcMillis = currentMillis;
    Serial.println("*** Relay OFF. Energy RESET. ***");

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 10);
    display.println("!! THRESHOLD HIT !!");
    display.setCursor(5, 30);
    display.println("Relay OFF");
    display.setCursor(5, 45);
    display.println("Energy Reset: 0 Wh");
    display.display();
    delay(5000);

    // Relay reactivation logic (Example: keep off, or add logic to re-enable after some condition)
    // relayState = true;
    // digitalWrite(RELAY_PIN, LOW);
  }

  display.setCursor(0, 55);
  display.print("Relay: ");
  display.println(relayState ? "ON" : "OFF");
  display.display();

  // --- SERIAL MONITOR OUTPUT ---
  Serial.println("===============================");
  Serial.print("Voltage: "); Serial.print(voltage, 2); Serial.println(" V");
  Serial.print("Current: "); Serial.print(current, 3); Serial.println(" A");
  Serial.print("Power: ");   Serial.print(power, 2);   Serial.println(" W");
  Serial.print("Energy: ");  Serial.print(accumulatedEnergy, 4); Serial.println(" Wh");
  Serial.print("Power Factor: "); Serial.println(pf, 2);
  Serial.print("Threshold: "); Serial.print(ENERGY_THRESHOLD, 1); Serial.println(" Wh");
  Serial.print("Relay: "); Serial.println(relayState ? "ON" : "OFF");

  delay(2000);
}