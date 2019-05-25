#include "sys/stream/SeekableIOStream.hpp"

// public
SeekableIOStream::~SeekableIOStream() {};

void SeekableIOStream::rewind() {
    seek(0);
}
