# AFE4950 Library API Documentation

## Overview

The `AFE4950` Arduino-compatible library allows easy configuration and data acquisition from the Texas Instruments AFE4950 analog front-end. It supports single-channel ECG and PPG simultaneous acquisition with optimized FIFO interrupt handling and internal ring buffer data structure for robust streaming.

* **Supported MCUs**: ESP32, nRF52, STM32, Arduino boards, etc.
* **Communication**: SPI
* **Data Handling**: 224-byte packets, internally buffered
* **Buffer Capacity**: Default internal buffer holds up to 20 packets (4480 bytes)

---

## Constructor

### `AFE4950()`

Constructs the `AFE4950` object and initializes internal structures. The internal circular buffer is automatically prepared.

#### Syntax

```cpp
AFE4950 afe;
```

---

## Methods

### `bool configure(uint8_t pinSCLK, uint8_t pinMISO, uint8_t pinMOSI, uint8_t pinCS, uint8_t pin_AFE_RST, uint8_t pin_FIFO_RDY)`

Initializes SPI and GPIO configuration to interface with the AFE4950.

#### Syntax

```cpp
bool configure(pinSCLK, pinMISO, pinMOSI, pinCS, pin_AFE_RST, pin_FIFO_RDY);
```

#### Parameters

* `pinSCLK`: SPI Clock pin.
* `pinMISO`: SPI MISO (Master In Slave Out) pin.
* `pinMOSI`: SPI MOSI (Master Out Slave In) pin.
* `pinCS`: Chip Select pin.
* `pin_AFE_RST`: Reset signal pin.
* `pin_FIFO_RDY`: Interrupt pin triggered when FIFO watermark is reached.

#### Returns

* `true` if configuration was successful.
* `false` otherwise.

---

### `void enableCapture()`

Starts the data acquisition by enabling the FIFO interrupt on the AFE4950.

#### Syntax

```cpp
void enableCapture();
```

---

### `void disableCapture()`

Stops the data acquisition by disabling the FIFO interrupt.

#### Syntax

```cpp
void disableCapture();
```

---

## Data Access and Buffer Management

### `size_t availableBytes() const`

Returns the number of bytes currently stored in the internal buffer and ready to be read.

#### Syntax

```cpp
size_t availableBytes() const;
```

#### Returns

* Number of bytes available in the ring buffer.

---

### `bool readByte(uint8_t &b)`

Reads a single byte from the internal ring buffer.

#### Syntax

```cpp
bool readByte(uint8_t &b);
```

#### Parameters

* `b`: Reference where the read byte will be stored.

#### Returns

* `true` if a byte was read successfully.
* `false` if the buffer was empty.

---

### `bool readBytes(uint8_t *dst, size_t len)`

Reads multiple bytes from the buffer.

#### Syntax

```cpp
bool readBytes(uint8_t *dst, size_t len);
```

#### Parameters

* `dst`: Pointer to the destination buffer.
* `len`: Number of bytes to read.

#### Returns

* `true` if the requested number of bytes was read.
* `false` if not enough data was available.

---

### `void clear()`

Clears the internal buffer, removing all unread data.

#### Syntax

```cpp
void clear();
```

---

## Deprecated Methods (v1.x compatibility)

The following methods were part of v1.x and are now replaced by ring buffer logic:

### `size_t getBytesToSend()` → use `availableBytes()` instead.

### `bool ready()` → deprecated, use `availableBytes() >= 224`.

### `const uint8_t* getData()` → deprecated, use `readBytes()`.

---

## Notes

* Internally, the buffer supports 4480 bytes (20 packets).
* Each packet has a header, 54 samples (ECG/PPG), and tagging for parsing.
* Sampling rates: ECG 500 Hz, PPG 250 Hz.

For examples and usage, see `/examples` directory in the library.

---

© Juan José Londoño Carrillo — Biomedical Engineer

