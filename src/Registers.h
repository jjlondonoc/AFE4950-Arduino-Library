/**
 * @file Registers.h
 * @brief Register configuration and structure definition for the AFE4950 device.
 * 
 * This file provides:
 * - Default register configuration settings for the AFE4950:
 *   - Watermark level of FIFO = 54 words
 *   - Acquisition mode: MIX (ECG and PPG)
 *   - LED used: TX1
 *   - Photodiode used: PD1
 *   - Frame rate (FPRF): 500 Hz
 *   - Sampling frequency:
 *     - ECG: 500 Hz
 *     - PPG: 250 Hz (due to decimation factor = 2)
 *   - Internal clocking mode: 128 kHz
 *   - Interrupt frequency: 13.8 Hz
 *   - Data length per interrupt: 224 bytes
 * 
 * - Definition of the basic register structure used to configure the AFE4950.
 * 
 * @note This file is intended for internal use by the AFE4950 library.
 */

 #ifndef __REGISTERS_H__
 #define __REGISTERS_H__
 
 #include <Arduino.h>
 
 struct Register {
     uint8_t address;
     uint32_t data;
 };
 
 extern Register page_1_registers[];
 extern size_t page_1_registers_size;
 
 extern Register page_0_registers[];
 extern size_t page_0_registers_size;
 
 #endif // __REGISTERS_H__