#ifndef LINUX_COMMANDLINE_HPP_LOCK
#define LINUX_COMMANDLINE_HPP_LOCK

#include "sys/Bool.hpp"
#include "sys/String.hpp"

#include "sys/collection/PropertyContainer.hpp"
#include "sys/collection/MutableCollection.hpp"

class CommandLine: public PropertyContainer {
    protected:
    MutableCollection<String> &_arguments;
    MutableMap<String, String> &_optionAliases;
    MutableMap<String, MutableCollection<String>> &_optionSets;
    Bool &True;
    MutableCollection<String> &Empty;
    
    virtual void registerOptionAlias(const char *option, const char *alias); 
    virtual void registerOptionSet(const char *option);
    virtual void setOption(String &option); 
    virtual void setOption(String &option, String &value); 
        
    public:
    CommandLine(Environment & env, MemoryInfo & mi);
    virtual ~CommandLine();
    
    virtual bool parse(int argc, char* argv[]);
    inline Iterator<String> & arguments() { return _arguments.iterator(); }
    
    virtual int optionSetSize(const char *option);
    virtual Iterator<String> & optionSet(const char *option);
};

// http://docopt.org/

#endif //LINUX_COMMANDLINE_HPP_LOCK
