#include "linux/CommandLine.hpp"

#include "sys/collection/LinkedList.hpp"

#define MODE_START 0
#define MODE_SHORT_OPT 1
#define MODE_LONG_OPT 2
#define MODE_OPT_VALUE 3
#define MODE_ARG 4

#define FAIL(message) {\
    env().err()<<(message)<<'\n';\
    return false;\
}

#define PUSH_ARG(arg) {\
    if (!(arg).length()) FAIL("no argument to push");\
    _arguments.add(createOwn<String, String&>((arg)));\
    (arg) = "";\
}
#define PUSH_OPT(opt) {\
    if (!(opt).length()) FAIL("no option to push");\
    setOption((opt));\
    (opt) = "";\
}
#define PUSH_OPT_VALUE(opt, value) {\
    if (!(opt).length() || !(value).length()) FAIL("no option with value to push");\
    setOption((opt), (value));\
    (opt) = "";\
    (value) = "";\
}

CommandLine::CommandLine(Environment & env, MemoryInfo & mi)
        :PropertyContainer(env,mi), Object(env,mi), 
         _optionAliases(env.create<HashMap<String,String>>()),
         _optionSets(env.create<HashMap<String,MutableCollection<String>>>()),
         _arguments(env.create<LinkedList<String>>()),
         Empty(env.create<LinkedList<String>>()),
         True(env.create<Bool, bool>(true)) {
}
CommandLine::~CommandLine() {
    True.destroy();
    Empty.destroy();
    _arguments.destroy();
    _optionSets.destroy();
    _optionAliases.destroy();
}    

bool CommandLine::parse(int argc, char* argv[]) {
    int mode = MODE_START;
    String arg(env());
    String &opt = env().create<String>();
    String value(env());
    
    for (int argn = 1; argn < argc; argn++) {
        char *current = argv[argn];
        char *c = current++;
        switch (*c) {
            case '-':
                switch (mode) {
                    case MODE_START:
                        break;
                    case MODE_ARG:
                        PUSH_ARG(arg);
                        break;
                    case MODE_SHORT_OPT:
                    case MODE_LONG_OPT:
                        PUSH_OPT(opt);
                        break;
                    case MODE_OPT_VALUE:
                        PUSH_OPT_VALUE(opt, value);
                        break;
                }
                mode = MODE_SHORT_OPT;
                break;
            default:
                switch (mode) {
                    case MODE_START:
                        mode = MODE_ARG;
                        arg << *c;
                        break;
                    case MODE_ARG:
                        PUSH_ARG(arg);
                        arg << *c;
                        break;
                    case MODE_SHORT_OPT:
                    case MODE_LONG_OPT:
                        mode = MODE_OPT_VALUE;
                        value << *c;
                        break;
                    case MODE_OPT_VALUE:
                        PUSH_OPT_VALUE(opt, value);
                        mode = MODE_ARG;
                        arg << *c;
                        break;
                }
                break;
        }
        while (*(c = current++)) {
            switch (*c) {
                case '-':
                    switch (mode) {
                        case MODE_ARG:
                            arg << *c;
                            break;
                        case MODE_SHORT_OPT:
                            if (arg.length()) FAIL("'-' within short option list");
                            mode = MODE_LONG_OPT;
                            break;
                        case MODE_LONG_OPT:
                            if (!arg.length()) FAIL("'-' at start of long option");
                            opt << *c;
                            break;
                        case MODE_OPT_VALUE:
                            value << *c;
                            break;
                    }
                    break;
                case '=':
                    switch (mode) {
                        case MODE_ARG:
                            arg << *c;
                            break;
                        case MODE_SHORT_OPT:
                        case MODE_LONG_OPT:
                            mode = MODE_OPT_VALUE;
                            break;
                        case MODE_OPT_VALUE:
                            value << *c;
                            break;
                    }
                    break;
                default:
                    switch (mode) {
                        case MODE_ARG:
                            arg << *c;
                            break;
                        case MODE_SHORT_OPT:
                            if (opt.length()) { PUSH_OPT(opt); }
                            // fall through
                        case MODE_LONG_OPT:
                            opt << *c;
                            break;
                        case MODE_OPT_VALUE:
                            value << *c;
                            break;
                    }
                    break;
            }
        }
    }
    switch (mode) {
        case MODE_START:
            break;
        case MODE_ARG:
            PUSH_ARG(arg);
            break;
        case MODE_SHORT_OPT:
            if (!(opt).length()) {
                arg << '-';
                PUSH_ARG(arg);
                break;
            }
        case MODE_LONG_OPT:
            PUSH_OPT(opt);
            break;
        case MODE_OPT_VALUE:
            PUSH_OPT_VALUE(opt, value);
            break;
    }
    return true;
}

void CommandLine::setOption(String &option) {
    String &opt = _optionAliases.has(option) ? _optionAliases.get(option) : createOwn<String, String&>(option);
    set(opt, True);
}
void CommandLine::setOption(String &option, String &value) {
    String &opt = _optionAliases.has(option) ? _optionAliases.get(option) : createOwn<String, String&>(option);
    if (_optionSets.has(opt)) {
        _optionSets.get(opt).add(createOwn<String, String&>(value));
    } else {
        set(opt, createOwn<String, String&>(value));
    }
}
void CommandLine::registerOptionAlias(const char *option, const char *alias) {
    _optionAliases.set(createOwn<String, const char *>(alias), createOwn<String, const char *>(option));
} 
void CommandLine::registerOptionSet(const char *option) {
    _optionSets.set(createOwn<String, const char *>(option), env().create<LinkedList<String>>());
}

int CommandLine::optionSetSize(const char *option) {
    String &opt = env().create<String, const char*>(option); 
    int size = _optionSets.has(opt) ? _optionSets.get(opt).size() : 0;
    opt.destroy();
    return size;
}
Iterator<String> & CommandLine::optionSet(const char *option) {
    String &opt = env().create<String, const char*>(option); 
    Iterator<String> &it = (_optionSets.has(opt) ? _optionSets.get(opt) : Empty).iterator();
    opt.destroy();
    return it;
}
