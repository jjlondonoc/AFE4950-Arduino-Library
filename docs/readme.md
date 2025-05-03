# AFE4950 Library

The **AFE4950** library enables Arduino-compatible boards (such as the ESP32, nRF52, STM32, etc.) to interface with the **AFE4950 analog front-end**, a high-precision sensor designed for simultaneous **ECG** (Electrocardiogram) and **PPG** (Photoplethysmogram) signal acquisition.

This library provides a robust, object-oriented API for configuring the AFE4950, starting/stopping capture, and reading raw physiological data in an efficient and scalable manner. It is tailored for real-time biomedical signal acquisition and transmission.

---

## What's New in Version 2.0

* **Internal circular buffer (ring buffer)** for efficient data handling using embedded-friendly structures. Prevents FIFO overflow and decouples acquisition from data processing.
* **Multiplatform support**: Now compatible with ESP32, nRF52, STM32, and other microcontrollers.
* **Modular interrupt logic**: Uses `IRAM_ATTR` only when compiling for ESP32, enabling cross-platform usage.
* **Improved memory handling**: Buffer size can now be customized through internal definitions.
* **Deprecated**: Methods like `getData()`, `ready()`, and `getBytesToSend()` have been replaced with buffer-safe APIs:

  * `availableBytes()`
  * `readByte(...)`
  * `readBytes(...)`

---

## Internals

The library configures the AFE4950 to sample **ECG at 500 Hz** and **PPG at 250 Hz**, using its internal FIFO to generate an interrupt when **54 samples** are ready (13.8 Hz interrupt frequency). Each interrupt produces a structured **224-byte data packet**, stored in an internal ring buffer with capacity for up to 20 packets.

The internal structure ensures non-blocking acquisition and allows downstream processes (BLE, Serial, SD, etc.) to consume data at their own pace without loss.

---

## Getting Started

Include the library in your sketch:

```cpp
#include <AFE4950.h>
```

Configure the SPI and GPIO pins in your setup:

```cpp
AFE4950 afe;

void setup() {
  afe.configure(SCLK, MISO, MOSI, CS, AFE_RST, FIFO_RDY);
  afe.enableCapture(); // Start data acquisition
}
```

---

## Circuit

The AFE4950 module communicates via **SPI** and requires:

* Power (typically 3.3V or 5V depending on board)
* SPI connections (SCLK, MISO, MOSI, CS)
* Digital pins for **RESET** and **FIFO\_RDY interrupt**

It is essential to correctly wire the ECG electrodes and PPG photodiode. For pinout, electrical requirements, and schematic references, consult the hardware datasheet or documentation of the AFE4950 module you are using.

---

## Examples

* [`RawPacket-ECG-PPG-Streaming`](../examples/RawPacket-ECG-PPG-Streaming/RawPacket-ECG-PPG-Streaming.ino): Demonstrates raw packet capture and transmission via Serial.
* [`Decode-ECG-PPG`](../examples/Decode-ECG-PPG/Decode-ECG-PPG.ino): Shows how to parse and extract individual ECG and PPG samples from packets in real-time.

---

## Requirements

* **AFE4950 Module** with a compatible ECG+PPG analog front-end.
* A microcontroller with hardware SPI and external interrupt support (ESP32, nRF52840, STM32, etc.).
* Serial baud rate: Recommended `115200` or higher for stable raw streaming.
* Configure your MCU platform correctly in Arduino IDE or PlatformIO.

---

## Documentation

* [API Reference](api.md): Full list of public methods, arguments, and usage notes.
* [Registers.h](../src/Registers.h): Describes register-level configurations.
* [CircularBuffer.h](../src/CircularBuffer.h): Internal ring buffer used for decoupled data management.

---

## Author

**Juan José Londoño Carrillo**
Biomedical Engineer | Embedded Developer
License: MIT

