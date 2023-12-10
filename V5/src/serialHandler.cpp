#include "teensy/serialHandler.hpp"

#include "main.h"
#include "teensy/cobs.hpp"

SerialHandler::SerialHandler(int sendPort, int receivePort, int baudRate) {
    if (serialR != nullptr) delete serialR;
    if (serialS != nullptr) delete serialS;
    serialS = new pros::Serial(sendPort);
    serialR = new pros::Serial(receivePort);
    setBaudRate(baudRate);
}

// Set the baud rate of the serial ports
// Returns 1 on success, PROS_ERR on failure
int32_t SerialHandler::setBaudRate(int baudRate) {
    int32_t s = serialS->set_baudrate(baudRate);
    int32_t r = serialR->set_baudrate(baudRate);
    return s == PROS_ERR || r == PROS_ERR ? PROS_ERR : 1;
}

// Flush the serial ports
// Returns 1 on success, PROS_ERR on failure
int32_t SerialHandler::flush() {
    int32_t s = serialS->flush();
    int32_t r = serialR->flush();
    return s == PROS_ERR || r == PROS_ERR ? PROS_ERR : 1;
}

// Get the send serial port
pros::Serial *SerialHandler::getSendSerial() { return serialS; }

// Get the receive serial port
pros::Serial *SerialHandler::getReceiveSerial() { return serialR; }

// Get the Teensy data
TeensyData SerialHandler::getTeensyData() { return teensyData; }

// Reading callback in the form: COBS_read_callback
char cobsRead(size_t i, void *data) {
    pros::Serial *serialR = (pros::Serial *)data;
    int32_t c = serialR->read_byte();
    if (c == -1 || c == PROS_ERR)
        std::cout << "COBS serial read error" << std::endl;
    return (char)c;
}

// Writing callback in the form: COBS_write_callback
void cobsWrite(size_t i, char c, void *data) {
    pros::Serial *serialS = (pros::Serial *)data;
    int32_t w = serialS->write_byte(c);
    if (w == -1 || w == PROS_ERR)
        std::cout << "COBS serial write error" << std::endl;
}

// Send data to the Teensy
void SerialHandler::send(V5Data *data) {
    cobsSender.set(data);
    cobsSender.write(cobsWrite, (void *)serialS);
}

// Receive data from the Teensy
void SerialHandler::receive() {
    if (serialR->get_read_avail() > cobsReceiver.size()) {
        cobsReceiver.read(cobsRead, (void *)serialR);
        cobsReceiver.get(&teensyData);
        serialR->flush();
    }
}
