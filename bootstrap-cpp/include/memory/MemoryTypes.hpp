#ifndef MEMORYTYPES_HPP_LOCK
#define MEMORYTYPES_HPP_LOCK

#define MEMORY_INFO_MAGIC 0xE3E0

#define notAnInfo ((MemoryInfo *) -1)

typedef unsigned int size_t;

typedef struct {
    unsigned long used      : 1;
    unsigned long reserved  : 1;
    unsigned long align     : 14;
    unsigned long magic     : 16;
} MemoryFlags;

typedef struct MemoryInfo {
    void* buf;
    size_t len;
    MemoryFlags flags;
    void* owner;
    struct MemoryInfo* prev;
    struct MemoryInfo* next;
} MemoryInfo;

typedef struct MemoryInfoArray {
    int size;
    MemoryInfo elements[0];
} MemoryInfoArray;
#endif //MEMORYTYPES_HPP_LOCK
