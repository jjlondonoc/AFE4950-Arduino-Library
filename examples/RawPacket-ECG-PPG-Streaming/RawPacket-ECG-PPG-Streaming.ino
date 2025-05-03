/*
 * Example: Single-Channel ECG and PPG Acquisition with AFE4950 Module and ESP32
 * 
 * Description:
 * This example demonstrates how to use the AFE4950 Module with an ESP32 to acquire 
 * single-channel ECG and PPG signals simultaneously. The AFE4950 is configured 
 * with a sampling frequency of 500 Hz for ECG and 250 Hz for PPG, and the FIFO is 
 * set with a watermark level of 54 samples. The library creates an internal ring buffer
 * to temporarily store the acquired AFE4950 data packets before they are transmitted.
 * 
 * The buffer is sized to hold up to 20 complete data packets simultaneously,
 * ensuring that no data is lost even if the data consumer (e.g., BLE transmission or serial communication)
 * 
 * This mechanism provides robustness against brief interruptions, maintaining continuous and reliable data capture
 * from the AFE4950 without risking FIFO overflows or data loss.
 * 
 * Library: AFE4950
 * Author: Juan José Londoño Carrillo - Biomedical Engineer
 * License: MIT
 */

#include <AFE4950.h> // Include library

/*
 * Define MCU pins for SPI protocol
 * Define digital pin for AFE reset signal
 * Define digital pin for receive AFE interrupts
 */
#define SCLK 18
#define MISO 19
#define MOSI 23
#define CS 5
#define AFE_RST 4
#define FIFO_RDY 16

AFE4950 afe; // Create AFE object for control an AFE
bool captureEnabled = false; // Flag for control the data acquisition

/*
 * Size of a complete data packet acquired from the AFE4950:
 * 
 * Each packet consists of:
 * - 54 FIFO words (each word is a 24-bit sample) corresponding to ECG and PPG measurements.
 * - Each sample is stored using 3 bytes plus a 1-byte tag separator.
 * - An 8-byte header is added at the beginning of each packet.
 * 
 * Total size:
 * - (54 samples × 4 bytes per sample) + 8 bytes header = 224 bytes.
 */
static const uint8_t PACKET_SIZE = 224;

void setup() {
  Serial.begin(115200); // Begin Serial communication

  while(!afe.configure(SCLK, MISO, MOSI, CS, AFE_RST, FIFO_RDY)) { // Wait for AFE configuration
    Serial.println("[ERROR] AFE4950 configuration failed. Retrying...");
    delay(500);
  }

  Serial.println("[INFO] AFE4950 configuration successful!");
}

void loop() {
  // Handle Serial commands to start or stop acquisition
  if (Serial.available() > 0) { // Search for data available in Serial port
    String command = Serial.readStringUntil('\n'); // Get command after a \n
    command.trim(); // Trim special characters in command like \r

    if (command.equalsIgnoreCase("start")) {
      captureEnabled = true;
      afe.enableCapture(); // Enable data capture from the AFE into the MCU
      Serial.println("[INFO] Data acquisition started.");
    }

    else if (command.equalsIgnoreCase("stop")) {
      captureEnabled = false; 
      afe.disableCapture(); // Disable data capture
      Serial.println("[INFO] Data acquisition stopped.");
    } 
  }

  // If capturing, check if a full packet is available
  if (captureEnabled) {
    while(afe.availableBytes() >= PACKET_SIZE) {
      uint8_t packet[PACKET_SIZE]; // Create a buffer for storage complete packet from ring buffer
      afe.readBytes(packet, PACKET_SIZE); // Read a complete packet
      Serial.write(packet, PACKET_SIZE); // Transmit packet over Serial
    }
  }
}