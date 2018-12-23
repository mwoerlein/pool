#ifndef SYS_LOG_LOGGERAWARE_HPP_LOCK
#define SYS_LOG_LOGGERAWARE_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/log/Logger.hpp"

class LoggerAware: virtual public Object {
    protected:
    Logger *logger;
    
    public:
    LoggerAware(Environment &env, MemoryInfo &mi = *notAnInfo):Object(env, mi), logger(0) {}
    virtual ~LoggerAware() {}
    
    virtual void setLogger(Logger &logger) { this->logger = &logger; }
    
    inline OStream &crit()  { return logger->crit();  }
    inline OStream &error() { return logger->error(); }
    inline OStream &warn()  { return logger->warn();  }
    inline OStream &info()  { return logger->info();  }
    inline OStream &debug() { return logger->debug(); }
    
    inline OStream &log(log_level level) {
        return logger->log(level);
    }
};

#endif //SYS_LOG_LOGGERAWARE_HPP_LOCK

