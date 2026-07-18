/*
 * Arduino Based ECG & Heartbeat Monitoring System
 * Microcontroller: Arduino Uno R3 (ATmega328P)
 * Sensor: AD8232 Single Lead Heart Rate Monitor
 *
 * This code reads the analog ECG signal from the AD8232 and the Leads-Off (LO) status.
 * It sends the data over Serial communication for visualization in the Processing IDE.
 */

// --- PIN DEFINITIONS ---
// Analog pin connected to the AD8232 OUTPUT pin
const int analogPin = A0; 
// Digital pins connected to the AD8232 Leads-Off pins
const int loPlusPin = 10;
const int loMinusPin = 11;

// --- SETUP FUNCTION ---
void setup() {
  // Start serial communication. MUST MATCH the Baud Rate in your Processing Sketch!
  Serial.begin(115200); 

  // Set the Leads-Off pins as digital inputs
  pinMode(loPlusPin, INPUT);
  pinMode(loMinusPin, INPUT);
}

// --- MAIN LOOP ---
void loop() {
  // Read the status of the Leads-Off pins
  int loPlusStatus = digitalRead(loPlusPin);
  int loMinusStatus = digitalRead(loMinusPin);

  /* * LEADS-OFF DETECTION LOGIC:
   * When the electrodes are NOT properly connected, the AD8232 sets LO+ and/or LO- to HIGH (1).
   * We use this to output an error signal.
   */
  if (loPlusStatus == HIGH || loMinusStatus == HIGH) {
    // If LEADS-OFF is detected (bad contact), send a high number (e.g., 1100).
    // This value is outside the normal 0-1023 range, signaling an error to the Processing sketch.
    Serial.println(1100); 
  } else {
    // If LEADS-OFF is NOT detected (good contact), read the ECG analog signal.
    // The analogRead() returns a value between 0 and 1023 (10-bit resolution).
    int ecgValue = analogRead(analogPin);
    
    // Output the raw ECG reading value (0-1023) to the serial port.
    Serial.println(ecgValue);
  }

  // A small delay to keep the data stream stable and prevent flooding the serial port.
  delay(1); 
}