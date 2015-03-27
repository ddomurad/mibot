#ifndef SDA_H
#define SDA_H

#include <QtCore>

/*
 *      Drive Protocol
 *  -------------------
 *
 *  DATA ORDER:
 *      CMD | ADDR | DATA
 *
 *      CMD - command ( like READ_REG, WRITE_REG )
 *      ADDR - usualy the addres of an register. ( like the left pwm register )
 *      DATA - data to wrtie ( if write command )
 *
 *  COMMANDS:
 * --------------------------
 *
 *  READ_REG:   0x0N
 *  Read up to 16 registers - begining from given addr. N = {0 .. F}
 *
 *  WRITE_REG:  0xN0
 *  Write up to 16 registers - begining from given addr. N = {0 .. F}
 *
 *  READABLE_REGISTERS:
 *
 *  WRITEABLE_REGISTERS:
 *
 */

namespace mibot
{
#define RWP_IS_READ_COMMAND(C) ((C & 0x0F) != 0)
#define RWP_IS_WRITE_COMMAND(C) ((C & 0xF0) != 0)
#define RWP_BYTES_TO_READ(C) ((C & 0x0F) + 1)
#define RWP_BYTES_TO_WRITE(C) ( (C >> 4)  + 1)

class ReadWriteBytesProtocol
{
    enum class ProtocolState
    {
        ReadCommand = 0,
        ReadWriteAddr,
        WriteData,
        ProcessRead,
    };
public:
    ReadWriteBytesProtocol();
    ~ReadWriteBytesProtocol();

    void InitRegisters(uchar writeable, uchar readable);
    void SetWritableAddr(uchar addr, qint8 * ptr);
    void SetReadableAddr(uchar addr, qint8 * ptr);

    void PushByte(qint8 c);
    qint8 *FlushOutputBuffer();
    uchar BytesToRead();

private:
    uchar   _writable_size;
    qint8 ** _writable;

    uchar   _readable_size;
    qint8 ** _readable;

    uchar _bytes_to_read;
    qint8 * _read_buffer;

    void setOutputBuffer(uchar addr, uchar cnt);

    ProtocolState _protocol_state;

    qint8 _chaced_command;
    uchar _chaced_write_addr;

    uchar _bytes_to_write_left;
};

}

#endif // SDA_H
