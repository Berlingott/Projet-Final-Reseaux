//
// Created by Berlingot on 2023-04-21.
//
#include "GestionPacket.h"
Packet::Packet()
= default;

Packet::Packet(char * _buffer, int _size)
{
    buffer = _buffer;
    size = _size;
}

Packet::Packet(const Packet & p) //Allocate new block for buffer
{
    size = p.size;
    buffer = new char[size];
    memcpy(buffer, p.buffer, size);
}