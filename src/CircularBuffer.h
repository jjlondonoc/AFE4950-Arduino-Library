/**
 * @file CircularBuffer.h
 * @brief Simple circular buffer (ring buffer) implementation for byte storage.
 * 
 * Provides an efficient way to store and retrieve bytes in a first-in, first-out (FIFO) manner.
 * 
 * Used internally by the AFE4950 library to manage continuous data streams
 * without risking data loss due to blocking operations.
 * 
 * @note This file is intended for internal use within the AFE4950 library.
 * End users typically do not interact with this class directly.
 */

#ifndef __CIRCULARBUFFER_H__
#define __CIRCULARBUFFER_H__

#include <Arduino.h>

class CircularBuffer {
    public:
        CircularBuffer(uint8_t* buffer, size_t capacity);

        /**
         * @brief Inserts a byte into the circular buffer.
         * 
         * If the buffer is full, the oldest data will be overwritten to make space.
         * 
         * @param data Byte to be inserted into the buffer.
         */
        void push(uint8_t data);

        /**
         * @brief Retrieves and removes a byte from the circular buffer.
         * 
         * @param[out] data Variable to store the retrieved byte.
         * @return true if a byte was successfully retrieved, false if the buffer was empty.
         */
        bool pop(uint8_t &data);

        /**
         * @brief Clears the circular buffer.
         * 
         * Resets the head and tail pointers, effectively discarding all stored data.
         */
        void clear();

        /**
         * @brief Returns the number of bytes currently stored in the buffer.
         * 
         * @return Number of bytes available to read from the buffer.
         */
        size_t size() const;
        
        size_t capacity() const;
        bool isEmpty() const;
        bool isFull() const;

    private:

        uint8_t* _buffer;
        size_t _capacity;
        volatile size_t _head;
        volatile size_t _tail;

};

#endif // __CIRCULARBUFFER_H__