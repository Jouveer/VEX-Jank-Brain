#ifndef COBS_HPP
#define COBS_HPP

#include <cstdlib>
#include <cstring>
#include <stdexcept>

/*! Macro for declaration of struct to insert specific packed data into
 *  the union of the COBS class. The resulting struct will be packed
 *  X is the struct typedefinition and Y is the body of the struct.
 */
#define COBS_DEFINE_STRUCT(X, Y)        \
    struct X Y __attribute__((packed)); \
    typedef struct X X;

/*! This union combines the data to send and a buffer to send the data. The
 * struct is accessed through the data field.
 */
template <class STRUCT>
union Packet {
    STRUCT data; /**< The struct or the data field (like an array) */
    unsigned char buffer[sizeof(
        STRUCT)]; /**< The buffer is the char array equivalent of the data */
};

/*! Read Callback: this is the prototype of function that should return a char
 * at the time. It has 2 arguments. The first, in the form of a size_t is the
 * current index in the reading, the second is a payload (arbitrary content) in
 * the form a void pointer.
 */
typedef char (*COBS_read_callback)(size_t, void*);
/*! Write Calback: this is the prototype of function that should accept one char
 * at the time. The callback has 3 arguments: the index of currently read char,
 * the char itself, and a payload (arbitrary content) in the form a void
 * pointer.
 */
typedef void (*COBS_write_callback)(size_t, char, void*);

/*! COBS Class
 *  The COBS class is an abstraction of the Consistent Overhead Byte Stuffing,
 * an algorithm for encoding data byte, in such a way the receiver should not
 * handle necessarely the dimension of the packet (the packet is ended when it
 * contains a 0x00). The class is abstracted over a template that contains the
 * actual data.
 */
template <class STRUCT>
class COBS {
    union Packet<STRUCT> packet; /**< The actual package */
    unsigned char* outbound; /**< The container for the COBS encoded package */

    void stuff(void);   /**< The encoding function */
    void unstuff(void); /**< The decoding function */

   public:
    /*! The class constructor */
    COBS() {
        outbound = (unsigned char*)malloc(sizeof(STRUCT) + 1);
        // if (!(outbound)) throw std::runtime_error("Allocation error");
    };

    /*! The class destructor */
    ~COBS() {
        if (outbound) free(outbound);
    }
    /*! Set the struct (or any other kind of data to be sent) */
    void set(const STRUCT* s) { memcpy(&(packet.data), s, sizeof(STRUCT)); }
    /*! Get the struct (or any other kind of data to be received) */
    void get(STRUCT* s) { memcpy(s, &(packet.data), sizeof(STRUCT)); }
    /*! Return the current COBS encoded package */
    const char* get_packet() const { return outbound; }
    /*! Get the size of the handled data */
    const size_t size() { return sizeof(STRUCT); }
    /*! Read from the transmission channel the COBS package */
    void read(COBS_read_callback read_callback, void* payload);
    /*! Write in the transmission channel the COBS package */
    void write(COBS_write_callback write_callback, void* payload);
};

#define FinishBlock(X) (*code_ptr = (X), code_ptr = dst++, code = 0x01)
template <class STRUCT>
void COBS<STRUCT>::stuff(void) {
    memset(outbound, 0, sizeof(STRUCT) + 1);

    const unsigned char* end = packet.buffer + sizeof(STRUCT);
    const unsigned char* ptr = packet.buffer;
    unsigned char* dst = outbound;

    unsigned char* code_ptr = dst++;
    unsigned char code = 0x01;

    while (ptr < end) {
        if (*ptr == 0)
            FinishBlock(code);
        else {
            *(dst)++ = *ptr;
            if (++code == 0xFF) FinishBlock(code);
        }
        ptr++;
    }
    FinishBlock(code);
}

template <class STRUCT>
void COBS<STRUCT>::unstuff(void) {
    const unsigned char* end = outbound + sizeof(STRUCT) + 1;
    unsigned char* ptr = outbound;
    unsigned char* dst = packet.buffer;

    while (ptr < end) {
        int code = *ptr++;
        for (int i = 1; ptr < end && i < code; i++) *dst++ = *ptr++;
        if (code < 0xFF) *dst++ = 0;
    }
}

template <class STRUCT>
void COBS<STRUCT>::read(COBS_read_callback read_callback, void* payload) {
    for (size_t i = 0; i < sizeof(STRUCT) + 1; i++)
        outbound[i] = read_callback(i, payload);
    unstuff();
}

template <class STRUCT>
void COBS<STRUCT>::write(COBS_write_callback write_callback, void* payload) {
    stuff();
    for (size_t i = 0; i < sizeof(STRUCT) + 1; i++) {
        write_callback(i, outbound[i], payload);
    }
}

#endif /* COBS_HPP */