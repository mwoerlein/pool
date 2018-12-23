#ifndef SYS_LOG_LOGGER_HPP_LOCK
#define SYS_LOG_LOGGER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/log/LineCountingOStream.hpp"

enum log_level {log_crit = 0, log_error = 1, log_warn = 2, log_info = 3, log_debug = 4};

class Logger: virtual public Object {
    protected:
    OStream &_nullStream;
    LineCountingOStream &_crit;
    LineCountingOStream &_error;
    LineCountingOStream &_warn;
    LineCountingOStream &_info;
    LineCountingOStream &_debug;
    
    public:
    Logger(Environment &env, MemoryInfo &mi = *notAnInfo, log_level level = log_info);
    virtual ~Logger();
    
    inline OStream &crit()  { return _crit;  }
    inline OStream &error() { return _error; }
    inline OStream &warn()  { return _warn;  }
    inline OStream &info()  { return _info;  }
    inline OStream &debug() { return _debug; }
    
    virtual OStream &log(log_level level = log_info);
    virtual int get(log_level level = log_info);
    virtual bool has(log_level level = log_info);
};

#endif //SYS_LOG_LOGGER_HPP_LOCK

