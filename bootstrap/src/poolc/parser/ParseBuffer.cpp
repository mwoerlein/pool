#include "poolc/parser/ParseBuffer.hpp"

#include "memory/MemoryInfoHelper.hpp"

// public
ParseBuffer::ParseBuffer(Environment &env, MemoryInfo &mi, size_t size, size_t lookahead):
    Object(env, mi),
    size(size),
    lookahead(lookahead),
    buffersInfo(env.getAllocator().allocate((size + lookahead) * (sizeof(char) + sizeof(int) + sizeof(int)))),
    buffer((char*) buffersInfo.buf),
    linesBuffer((int*) memoryEnd(buffersInfo.buf, (size + lookahead) * sizeof(char))),
    columnsBuffer((int*) memoryEnd(buffersInfo.buf, (size + lookahead) * (sizeof(char) + sizeof(int)))),
    bufferPos(-size),
    currentLine(1),
    currentColumn(1) {
    token = current = marker = ctxmarker = limit = buffer + size;
}

ParseBuffer::~ParseBuffer() {
    env().getAllocator().free(buffersInfo);
}

bool ParseBuffer::freeBuffer(size_t need) {
    const size_t free = token - buffer;
    const size_t filled = limit - token;
    if (free < need) {
        return false;
    }
    for (int i = 0; i < filled; i++) {
        buffer[i] = buffer[i + free];
        linesBuffer[i] = linesBuffer[i + free];
        columnsBuffer[i] = columnsBuffer[i + free];
    }
    limit -= free;
    current -= free;
    token -= free;
    marker -= free;
    ctxmarker -= free;
    bufferPos += free;
    shift(free);
    return true;
}

bool ParseBuffer::fillBuffer(size_t need, IStream & input)
{
    if (input.isEmpty() || !freeBuffer(need)) {
        return false;
    }
    char current;
    while (!input.isEmpty() && (limit < buffer + size)) {
        input >> current;
        linesBuffer[limit - buffer] = currentLine;
        columnsBuffer[limit - buffer] = currentColumn++;
        *limit++ = current;
        if (current == '\n') {
            currentLine++;
            currentColumn = 1;
        }
    }
    if (input.isEmpty()) {
        // clear lookahead after end of stream
        char * maxfill = limit + lookahead;
        while (limit < maxfill) {
            linesBuffer[limit - buffer] = 0;
            columnsBuffer[limit - buffer] = 0;
            *limit++ = (char) 0;
        }
    }
    return true;
}
