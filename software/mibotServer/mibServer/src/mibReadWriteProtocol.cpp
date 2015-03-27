#include "inc/mibReadWriteProtocol.h"

using namespace mibot;


ReadWriteBytesProtocol::ReadWriteBytesProtocol() :
    _writable(nullptr),
    _readable(nullptr),
    _bytes_to_read(0),
    _read_buffer(nullptr),
    _protocol_state(ProtocolState::ReadCommand)
{}

ReadWriteBytesProtocol::~ReadWriteBytesProtocol()
{
    if(_readable != nullptr)
        delete[] _readable;

    if(_writable != nullptr)
        delete[] _writable;

    if(_read_buffer != nullptr)
        delete[] _read_buffer;
}

void ReadWriteBytesProtocol::InitRegisters(uchar writeable, uchar readable)
{
    if(_readable != nullptr)
        delete[] _readable;

    if(_writable != nullptr)
        delete[] _writable;

    if(_read_buffer != nullptr)
        delete[] _read_buffer;

    _writable       = new qint8*[writeable];
    _readable       = new qint8*[readable];
    _read_buffer   = new qint8[readable];

    _writable_size = writeable;
    _readable_size = readable;
}

void ReadWriteBytesProtocol::SetWritableAddr(uchar addr, qint8 *ptr)
{
    _writable[addr] = ptr;
}

void ReadWriteBytesProtocol::SetReadableAddr(uchar addr, qint8 *ptr)
{
    _readable[addr] = ptr;
}

void ReadWriteBytesProtocol::PushByte(qint8 c)
{
    switch(_protocol_state)
    {
    case ProtocolState::ReadCommand : // push command
        _chaced_command = c;
        if( RWP_IS_READ_COMMAND(c) )
        {
            _protocol_state = ProtocolState::ProcessRead;
            return;
        }
        else if( RWP_IS_WRITE_COMMAND(c) )
        {
            _bytes_to_write_left = RWP_BYTES_TO_WRITE(c);
            _protocol_state = ProtocolState::ReadWriteAddr;
            return;
        }
        break;
    case ProtocolState::ReadWriteAddr: // push addr for write
        _chaced_write_addr = c;
        _protocol_state = ProtocolState::WriteData;
        break;

    case ProtocolState::WriteData: // write byte
        if(_chaced_write_addr < _writable_size)
            *(_writable[_chaced_write_addr]) = c;

        _chaced_write_addr++;
        _bytes_to_write_left--;
        if(_bytes_to_write_left == 0)
            _protocol_state = ProtocolState::ReadCommand;
        break;

    case ProtocolState::ProcessRead: // handle addr for read
        setOutputBuffer( c , RWP_BYTES_TO_WRITE(_chaced_command) );
        break;
    };
}

qint8 *ReadWriteBytesProtocol::FlushOutputBuffer()
{
    _bytes_to_read = 0;
    return _read_buffer;
}

uchar ReadWriteBytesProtocol::BytesToRead()
{
    return _bytes_to_read;
}

void ReadWriteBytesProtocol::setOutputBuffer(uchar addr, uchar cnt)
{
    for( int i=0; i<cnt; i++)
    {
        if(i + _bytes_to_read >= _readable_size)
        {
            qDebug() << "ReadWriteBytesProtocol Output buffer overflow";
            return;
        }

        if(addr + i < _readable_size)
            _read_buffer[i + _bytes_to_read] = *(_readable[addr + i]);
    }

    _bytes_to_read += cnt;
}
