#include "sys/log/Logger.hpp"

#include "sys/stream/NullIOStream.hpp"
#include "sys/log/LineCountingOStream.hpp"

// public
Logger::Logger(Environment &env, MemoryInfo &mi, log_level level)
        :Object(env, mi),
         _nullStream(env.create<NullIOStream>()),
         _crit (env.create<LineCountingOStream, OStream&>((level >= log_crit ) ? env.err(): _nullStream)),
         _error(env.create<LineCountingOStream, OStream&>((level >= log_error) ? env.err(): _nullStream)),
         _warn (env.create<LineCountingOStream, OStream&>((level >= log_warn ) ? env.err(): _nullStream)),
         _info (env.create<LineCountingOStream, OStream&>((level >= log_info ) ? env.out(): _nullStream)),
         _debug(env.create<LineCountingOStream, OStream&>((level >= log_debug) ? env.out(): _nullStream)) {
}
Logger::~Logger() {
    _crit.destroy();
    _error.destroy();
    _warn.destroy();
    _info.destroy();
    _debug.destroy();
    _nullStream.destroy();
}

OStream &Logger::log(log_level level) {
    switch (level) {
        case log_crit : return _crit;
        case log_error: return _error;
        case log_warn : return _warn;
        case log_info : return _info;
        case log_debug: return _debug;
    }
    return _error;
}

int Logger::get(log_level level) {
    switch (level) {
        case log_crit : return (int) _crit;
        case log_error: return (int) _error;
        case log_warn : return (int) _warn;
        case log_info : return (int) _info;
        case log_debug: return (int) _debug;
    }
    return 0;
}

bool Logger::has(log_level level) {
    switch (level) {
        case log_debug: if ((int) _debug) { return true; }
        case log_info : if ((int) _info)  { return true; }
        case log_warn : if ((int) _warn)  { return true; }
        case log_error: if ((int) _error) { return true; }
        case log_crit : if ((int) _crit)  { return true; }
    }
    return false;
}
