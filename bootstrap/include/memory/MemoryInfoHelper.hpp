#ifndef MEMORYINFOHELPER_HPP_LOCK
#define MEMORYINFOHELPER_HPP_LOCK

#define memoryEnd(buf, len) ((void *) ((size_t) (buf) + (len)))
#define memoryDiff(mem1, mem2) ((size_t) (mem2) - (size_t) (mem1))
#define memoryInfoEnd(info) ((void *) ((size_t) (info)->buf + (info)->len))
#define isMemoryInfo(ptr) (((MemoryInfo *) ptr)->flags.magic == MEMORY_INFO_MAGIC)
#define hasFollowingBuffer(ptr) (((MemoryInfo *) ptr)->buf == memoryEnd(ptr, sizeof(MemoryInfo)))
#define isEmbeddedInfo(ptr) (((MemoryInfo *) ptr)->buf == ((void *) ptr))
#define canEmbedInfo(ptr) (((MemoryInfo *) ptr)->len > sizeof(MemoryInfo))

#define isEmptyMemoryInfoList(list) ((list)->next == (list))

#define initMemoryInfoList(list) {\
    (list)->next = (list)->prev = (list);\
    (list)->buf = (void *) 0;\
    (list)->len = 0;\
    (list)->owner = (list);\
    (list)->flags.reserved = 0;\
    (list)->flags.used = 0;\
    (list)->flags.magic = MEMORY_INFO_MAGIC;\
}
    
#define unlinkMemoryInfo(info) {\
    (info)->next->prev = (info)->prev;\
    (info)->prev->next = (info)->next;\
    (info)->next = (info)->prev = (info);\
}

#define appendMemoryInfo(info, newInfo) {\
    (newInfo)->prev = (info);\
    (newInfo)->next = (info)->next;\
    (newInfo)->prev->next = (newInfo)->next->prev = (newInfo);\
}

#define replaceMemoryInfo(oldInfo, newInfo) {\
    (newInfo)->next = (oldInfo)->next;\
    (newInfo)->prev = (oldInfo)->prev;\
    (newInfo)->next->prev = (newInfo)->prev->next = (newInfo);\
    (oldInfo)->next = (oldInfo)->prev = (oldInfo);\
}

#endif //MEMORYINFOHELPER_HPP_LOCK
