#include "main.hpp"

#define HWSERIAL Serial1

SerialHandler::SerialHandler(int baudRate) { HWSERIAL.begin(baudRate); }

void cobsWrite(size_t i, char c, void *data) { HWSERIAL.write(c); }
char cobsRead(size_t i, void *data) { return (char)HWSERIAL.read(); }

void SerialHandler::send(TeensyData *data) {
    cobsSender.set(data);
    cobsSender.write(cobsWrite, nullptr);
}

void SerialHandler::receive() {
    if (unsigned(HWSERIAL.available()) > cobsReceiver.size()) {
        cobsReceiver.read(cobsRead, nullptr);
        cobsReceiver.get(&v5Data);
        // HWSERIAL.flush();
    }
}

V5Data SerialHandler::getV5Data() { return v5Data; }

void SerialHandler::flush() { HWSERIAL.flush(); }
