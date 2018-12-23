#include "linux/LinuxBootstrap.hpp"
#include "linux/CommandLine.hpp"
#include "linux/DirectoryPoolStorage.hpp"

#include "sys/log/Logger.hpp"
#include "poolc/parser/ClassLoader.hpp"
#include "poolc/ast/visitors/X86PasmVisitor.hpp"

static const char PROGRAM[] = "poolbc";
static const char VERSION[] = "0.1.0";
static const char USAGE[] =
R"(Pool Bootstrap Compiler.

    Usage:
      poolbc [options] -o <dir> -c <dir> [-c <dir>]... <fqn>...

    Options:
      -h --help                     Show this screen.
      --version                     Show version.
      -d --debug                    Set debug.
      -v --verbose                  Set verbose.
      --warning                     Set warning.
      --error                       Set error.
      -o <dir> --output <dir>       Place the (pasm) outputs into <dir>.
      -c <dir> --classpath <dir>    Search for classes in all of these directories.
)";

class PoolBootstrapCompilerCommand: public CommandLine {
    public:
    PoolBootstrapCompilerCommand(Environment & env, MemoryInfo & mi = *notAnInfo)
            :CommandLine(env, mi), Object(env, mi) {
        registerOptionAlias("help", "h");
        registerOptionAlias("verbose", "v");
        registerOptionAlias("debug", "d");
        registerOptionAlias("output", "o");
        registerOptionAlias("classpath", "c");
        
        registerOptionSet("classpath");
    }
    virtual ~PoolBootstrapCompilerCommand() {
    }
    
    int run(int argc, char* argv[]) {
        parse(argc, argv);
        
        if (hasProperty("help")) {
            env().out()<<USAGE;
            return 0;
        }
        if (hasProperty("version")) {
            env().out()<<PROGRAM<<" "<<VERSION<<"\n";
            return 0;
        }
        if (!hasStringProperty("output") || _arguments.size() < 1 || optionSetSize("classpath") < 1) {
            env().out()<<USAGE;
            return -1;
        }
        log_level ll = log_note;
        if (hasProperty("verbose")) ll = log_info; 
        if (hasProperty("debug")) ll = log_debug; 
        if (hasProperty("warning")) ll = log_warn; 
        if (hasProperty("error")) ll = log_error; 
        Logger &logger = env().create<Logger, log_level>(ll);
        ClassLoader &loader = env().create<ClassLoader>();
        loader.setLogger(logger);
        {
            Iterator<String> &it = optionSet("classpath");
            while (it.hasNext()) {
                loader.addStore(env().create<DirectoryPoolStorage, String&>(it.next()));
            }
            it.destroy();
        }
        
        DirectoryPoolStorage &outPS = env().create<DirectoryPoolStorage, String&>(getStringProperty("output"));
        Visitor &dump = env().create<X86PasmVisitor, PoolStorage &>(outPS);
        dump.setLogger(logger);
        
        {
            Iterator<String> &it = arguments();
            while (it.hasNext()) {
                String &name = it.next();
                logger.note() << "compile " << name << "\n";
                
                ClassDefNode * classDef = loader.getClass(name);
                if (!classDef || logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                if (classDef) {
                    classDef->accept(dump);
                    if (logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                }
            }
            it.destroy();
        }
        
        dump.destroy();
        outPS.destroy();
        loader.destroy();
        logger.destroy();
        
        return 0;
    }
};

int main(int argc, char* argv[]) {
    PoolBootstrapCompilerCommand command(LinuxBootstrap::buildEnvironment());
    return command.run(argc, argv);
}
