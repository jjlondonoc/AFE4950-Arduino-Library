#include "CircularBuffer.h"

CircularBuffer::CircularBuffer(uint8_t* buffer, size_t capacity) 
{
    _buffer = buffer;
    _capacity = capacity;
    _head = 0;
    _tail = 0;
}

void CircularBuffer::push(uint8_t data)
{
    size_t nextHead = (_head + 1) % _capacity;

    if (nextHead == _tail) {
        _tail = (_tail + 1) % _capacity; // Overwrite
    }

    _buffer[_head] = data;
    _head = nextHead;
}


bool CircularBuffer::pop(uint8_t &data)
{
    // If is empty
    if (_tail == _head) return false;

    data = _buffer[_tail];
    _tail = (_tail + 1) % _capacity;
    return true;
}

void CircularBuffer::clear()
{
    _head = 0;
    _tail = 0;
}

size_t CircularBuffer::size() const
{
    if (_head >= _tail) return _head - _tail;
    else return (_capacity - _tail) + _head;
}

size_t CircularBuffer::capacity() const
{
    return _capacity;
}

bool CircularBuffer::isEmpty() const
{
    return (_head == _tail);
}

bool CircularBuffer::isFull() const
{
    // Is full if nextHead == tail
    size_t nextHead = (_head + 1) % _capacity;
    return (nextHead == _tail);
}
