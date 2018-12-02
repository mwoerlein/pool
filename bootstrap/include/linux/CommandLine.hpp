#ifndef LINUX_COMMANDLINE_HPP_LOCK
#define LINUX_COMMANDLINE_HPP_LOCK

#include "sys/Bool.hpp"
#include "sys/String.hpp"

#include "sys/collection/PropertyContainer.hpp"
#include "sys/collection/MutableCollection.hpp"

class CommandLine: public PropertyContainer {
    protected:
    MutableCollection<String> &_arguments;
    Bool &True;
        
    public:
    CommandLine(Environment & env, MemoryInfo & mi);
    virtual ~CommandLine();
    
    virtual bool parse(int argc, char* argv[]);
    inline Iterator<String> & arguments() { return _arguments.iterator(); }
};

// http://docopt.org/

#endif //LINUX_COMMANDLINE_HPP_LOCK
