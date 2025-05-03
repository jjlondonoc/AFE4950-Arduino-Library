/**
 * @brief Driver for the AFE4950 Analog Front-End for ECG/PPG acquisition.
 * 
 * This class provides an interface to configure the AFE4950, handle data capture via interrupts,
 * and buffer the acquired samples internally using a circular buffer.
 * 
 * Internal settings:
 * - FIFO Watermark Level: 54 words
 * - Sampling Rate: ECG 500Hz, PPG 250Hz
 * - Interrupt frequency: ~13.8Hz
 * - Internal ring buffer size: 4480 bytes
 * 
 * Each data packet consists of:
 * - 224 bytes (54 samples with headers and tags)
 * 
 * The internal ring buffer can store approximately 19 full packets before overflow starts
 * (automatic overwriting of oldest data).
 * 
 * @note The user does not need to manage memory manually; the buffer is allocated internally.
 */

 #ifndef __AFE4950_H__
 #define __AFE4950_H__
 
 #include <Arduino.h>
 #include <Registers.h>
 #include "CircularBuffer.h"
 
 #define SPI_FREQ 4000000 // Clock frequency for SPI Bus
 
 class AFE4950 {
 
     public:
 
         /**
          * @brief Constructs a new AFE4950 object and initializes the internal circular buffer.
          */
         AFE4950();
 
         /**
          * @brief Destructor for the AFE4950 object.
          * 
          * Cleans up internal resources.
          */
         ~AFE4950();
 
         /**
          * @brief Configures the AFE4950 interface and initializes the necessary pins and SPI settings.
          * 
          * @param pinSCLK Pin number for SPI Clock (SCLK).
          * @param pinMISO Pin number for SPI Master In Slave Out (MISO).
          * @param pinMOSI Pin number for SPI Master Out Slave In (MOSI).
          * @param pinCS Pin number for SPI Chip Select (CS).
          * @param pin_AFE_RST Pin number connected to the AFE4950 RESET signal.
          * @param pin_FIFO_RDY Pin number connected to the AFE4950 FIFO ready interrupt signal.
          * @return true if the configuration was successful, false otherwise.
          */
         bool configure(
             const uint8_t pinSCLK,
             const uint8_t pinMISO,
             const uint8_t pinMOSI,
             const uint8_t pinCS,
             const uint8_t pin_AFE_RST,
             const uint8_t pin_FIFO_RDY
         );
 
         /**
          * @brief Starts the data capture process from the AFE4950 device.
          * 
          * This enables the FIFO ready interrupt and prepares the device for continuous data acquisition.
          */
         void enableCapture();
 
         /**
          * @brief Stops the data capture process from the AFE4950 device.
          * 
          * This disables the FIFO ready interrupt and halts data acquisition.
          */
         void disableCapture();
 
 
         // Data structure
 
         /**
          * @brief Returns the number of bytes currently available in the internal circular buffer.
          * 
          * @return Number of bytes available for reading.
          */
         size_t availableBytes() const;
 
         /**
          * @brief Reads a single byte from the internal circular buffer.
          * 
          * @param[out] b Variable to store the byte read.
          * @return true if a byte was successfully read, false if the buffer was empty.
          */
         bool readByte(uint8_t &b);
 
         /**
          * @brief Reads multiple bytes from the internal circular buffer.
          * 
          * @param[out] dst Pointer to the array where the read bytes will be stored.
          * @param[in] len Number of bytes to read.
          * @return true if all requested bytes were successfully read, false if not enough data was available.
          */
         bool readBytes(uint8_t *dst, size_t len);
 
         /**
          * @brief Clears the internal circular buffer, discarding all stored data.
          */
         void clear();
         
     private:
 
         uint8_t _pinSCLK;
         uint8_t _pinMISO;
         uint8_t _pinMOSI;
         uint8_t _pinCS;
         uint8_t _pin_AFE_RST;
         uint8_t _pin_FIFO_RDY;
 
         static AFE4950* _afeInstance;
         static const size_t RING_BUFFER_SIZE = 4480; // 10 packets
         static uint8_t _dataBuffer[RING_BUFFER_SIZE];
         static CircularBuffer _ringBuffer; // Internal data structure
         
         Register _reg0 = {0x00, 0x000000};  // Read/Write mode control
         Register _reg1 = {0x01, 0x000000};  // Page select control
 
         enum Page: uint8_t {
             page0,
             page1
         };
 
         enum Mode: uint8_t {
             write,
             read
         };
 
         enum CaptureStatus {
             STOPPED,
             RUNNING
         };
         
         CaptureStatus _captureStatus = STOPPED;
 
         void switchPage(Page page);             // Register 0x01
         void switchReadWriteMode(Mode mode);    // Register 0x00
 
         void writeSPI(const Register& reg);
         uint32_t readSPI(uint8_t regAddress);
         void writeRegister(const Register& reg);
         uint32_t readRegister(uint8_t regAddress);
 
         bool validateRegisters();
         void fifoConfig();
 
         void softwareReset();
         void reset();
 
         static void dataReadyISR();
         void copyDataInTxBuff(uint32_t data, uint8_t identifier);
         void readFifo();
 };
 
 #endif // __AFE4950_H__