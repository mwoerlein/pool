#include "linux/LinuxBootstrap.hpp"
#include "linux/CommandLine.hpp"
#include "linux/DirectoryPoolStorage.hpp"

#include "sys/log/Logger.hpp"
#include "poolc/parser/ClassLoader.hpp"
#include "poolc/pir/PIRGenerator.hpp"
#include "poolc/ast/visitors/PrettyPrinter.hpp"
#include "poolc/ast/visitors/MethodResolver.hpp"
#include "poolc/ast/visitors/TypeResolver.hpp"
#include "poolc/backend/x86/X86Writer.hpp"

static const char PROGRAM[] = "poolbc";
static const char VERSION[] = "0.1.0";
static const char USAGE[] =
R"(Pool Bootstrap Compiler.

    Usage:
      poolbc [options] -o <dir> [-p <dir>] -c <dir> [-c <dir>]... <fqn>...

    Options:
      -h --help                     Show this screen.
      --version                     Show version.
      -d --debug                    Set debug.
      -v --verbose                  Set verbose.
      --warning                     Set warning.
      --error                       Set error.
      -r --recursive                Generate output for all required classes.
      --resolveClasses              Generate static resolved classes.
      -o <dir> --output <dir>       Place the (pasm) outputs into <dir>.
      -p <dir> --prettyprint <dir>  Dump classes into <dir>.
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
        registerOptionAlias("prettyprint", "p");
        registerOptionAlias("classpath", "c");
        registerOptionAlias("recursive", "r");
        
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
        TypeManager &types = env().create<TypeManager>();
        ClassPathStorage &classPath = env().create<ClassPathStorage>();
        {
            Iterator<String> &it = optionSet("classpath");
            while (it.hasNext()) {
                classPath.addStore(env().create<DirectoryPoolStorage, String&>(it.next()));
            }
            it.destroy();
        }
        ClassLoader &loader = env().create<ClassLoader, ClassPathStorage&, TypeManager&>(classPath, types);
        loader.setLogger(logger);
        
        PrettyPrinter *pretty = 0;
        if (hasStringProperty("prettyprint")) {
            DirectoryPoolStorage &prettyPS = env().create<DirectoryPoolStorage, String&>(getStringProperty("prettyprint"));
            pretty = &env().create<PrettyPrinter, PoolStorage &>(prettyPS);
            pretty->setLogger(logger);
            loader.setPrettyPrint(*pretty);
        }
        
        Visitor &resolveMethods = env().create<MethodResolver>();
        resolveMethods.setLogger(logger);
        Visitor &resolveTypes = env().create<TypeResolver>();
        resolveTypes.setLogger(logger);
        Visitor &generatePIR = env().create<PIRGenerator, TypeManager&>(types);
        generatePIR.setLogger(logger);
        DirectoryPoolStorage &outPS = env().create<DirectoryPoolStorage, String&>(getStringProperty("output"));
        Visitor &generatePASM = env().create<X86Writer, PoolStorage &, bool>(outPS, hasProperty("resolveClasses"));
        generatePASM.setLogger(logger);
        
        {
            Iterator<String> *it = &arguments();
            if (hasProperty("recursive")) {
                while (it->hasNext()) {
                    String &name = it->next();
                    ClassDeclNode * classDef = loader.getClass(name);
                    if (!classDef || logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                }
                it->destroy();
                it = &loader.classNames();
            }
            while (!logger.has(log_error) && it->hasNext()) {
                String &name = it->next();
                logger.note() << "compile " << name << "\n";
                
                ClassDeclNode * classDef = loader.getClass(name);
                if (!classDef || logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                if (classDef) {
                    classDef->accept(resolveMethods);
                    if (logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                    classDef->accept(resolveTypes);
                    if (logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                    classDef->accept(generatePIR);
                    classDef->accept(dump);
                    if (logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                }
            }
            it->destroy();
        }
        
        generatePASM.destroy();
        outPS.destroy();
        generatePIR.destroy();
        resolveTypes.destroy();
        resolveMethods.destroy();
        if (pretty) { pretty->destroy(); }
        loader.destroy();
        classPath.destroy();
        types.destroy();
        logger.destroy();
        
        return 0;
    }
};

int main(int argc, char* argv[]) {
    PoolBootstrapCompilerCommand command(LinuxBootstrap::buildEnvironment());
    return command.run(argc, argv);
}
