#ifndef SYS_LOG_LOGGER_HPP_LOCK
#define SYS_LOG_LOGGER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/log/LineCountingOStream.hpp"

enum log_level {log_crit = 0, log_error, log_warn, log_note, log_info, log_debug};

class Logger: virtual public Object {
    protected:
    OStream &_nullStream;
    LineCountingOStream &_crit;
    LineCountingOStream &_error;
    LineCountingOStream &_warn;
    LineCountingOStream &_note;
    LineCountingOStream &_info;
    LineCountingOStream &_debug;
    
    public:
    Logger(Environment &env, MemoryInfo &mi = *notAnInfo, log_level level = log_note);
    virtual ~Logger();
    
    inline OStream &crit()  { return _crit;  }
    inline OStream &error() { return _error; }
    inline OStream &warn()  { return _warn;  }
    inline OStream &note()  { return _note;  }
    inline OStream &info()  { return _info;  }
    inline OStream &debug() { return _debug; }
    
    virtual OStream &log(log_level level = log_note);
    virtual int get(log_level level = log_note);
    virtual bool has(log_level level = log_note);
};

#endif //SYS_LOG_LOGGER_HPP_LOCK

