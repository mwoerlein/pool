#include "linux/LinuxBootstrap.hpp"
#include "linux/CommandLine.hpp"
#include "linux/DirectoryPoolStorage.hpp"

#include "sys/log/Logger.hpp"

#include "pasm/i386/PasmParser.hpp"

#include "poolc/ast/visitors/MethodResolver.hpp"
#include "poolc/ast/visitors/PrettyPrinter.hpp"
#include "poolc/ast/visitors/TypeResolver.hpp"
#include "poolc/backend/x86/X86Writer.hpp"
#include "poolc/parser/ClassLoader.hpp"
#include "poolc/pir/PIRGenerator.hpp"
#include "poolc/storage/Types.hpp"
#include "poolc/storage/OStreamPoolStorage.hpp"

static const char PROGRAM[] = "poolbsc";
static const char VERSION[] = "0.1.0";
static const char USAGE[] =
R"(Pool Bootstrap Static Compiler.

    Usage:
      poolbsc [options] -o <file> -b <fqn> -c <dir> [-c <dir>]... <fqn>...

    Options:
      -h --help                     Show this screen.
      --version                     Show version.
      -d --debug                    Set debug.
      -v --verbose                  Set verbose.
      --warning                     Set warning.
      --error                       Set error.
      -b <fqn> --binding <fqn>      Use header/footer from <fqn>.
      -o <file> --output <file>     Place the output into <file>.
      -c <dir> --classpath <dir>    Search for classes in all of these directories.
)";

class PoolBootstrapStaticCompilerCommand: public CommandLine {
    public:
    PoolBootstrapStaticCompilerCommand(Environment & env, MemoryInfo & mi = *notAnInfo)
            :CommandLine(env, mi), Object(env, mi) {
        registerOptionAlias("help", "h");
        registerOptionAlias("verbose", "v");
        registerOptionAlias("debug", "d");
        registerOptionAlias("binding", "b");
        registerOptionAlias("output", "o");
        registerOptionAlias("classpath", "c");
        
        registerOptionSet("classpath");
    }
    virtual ~PoolBootstrapStaticCompilerCommand() {
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
        if (!hasStringProperty("output") || !hasStringProperty("binding") || _arguments.size() < 1 || optionSetSize("classpath") < 1) {
            env().out()<<USAGE;
            return -1;
        }
        log_level ll = log_note;
        if (hasProperty("verbose")) ll = log_info; 
        if (hasProperty("debug")) ll = log_debug; 
        if (hasProperty("warning")) ll = log_warn; 
        if (hasProperty("error")) ll = log_error; 
        Logger &logger = env().create<Logger, log_level>(ll);
        
        ClassPathStorage &classPath = env().create<ClassPathStorage>();
        {
            Iterator<String> &it = optionSet("classpath");
            while (it.hasNext()) {
                String &cp = it.next();
                classPath.addStore(env().create<DirectoryPoolStorage, String&>(cp));
            }
            it.destroy();
        }
        
        StorageElement *header = classPath.getHeader(getStringProperty("binding"));
        StorageElement *footer = classPath.getFooter(getStringProperty("binding"));
        if (!header || !footer) {
            logger.error()<<"Invalid binding " << getStringProperty("binding") << "\n";
            return -1;
        }
        
        DirectoryPoolStorage &tmp = env().create<DirectoryPoolStorage, const char *>("/tmp");
        String &tmpString = tmp.getTemporaryClassname(PROGRAM);
        String &tmpMime = env().create<String, const char *>(MIMETYPE_PASM);
        OStream &tmpOut = tmp.writeElement(tmpString, tmpMime);
        {
            logger.debug() << "add header\n";
            IStream &in = header->getContent();
            tmpOut<<in<<"\n";
            in.destroy();
        }
        compileClasses(classPath, tmpOut, logger);
        {
            logger.debug() << "add footer\n";
            IStream &in = footer->getContent();
            tmpOut<<in<<"\n";
            in.destroy();
        }
        tmpOut.destroy();
        
        footer->destroy();
        header->destroy();
        classPath.destroy();
        if (logger.has(log_error)) { return 1; }
        
        StorageElement *tmpElement = tmp.getElement(tmpString, tmpMime);
        IStream &in = tmpElement->getContent();
        OStream &out = env().streamFactory().buildOStream(getStringProperty("output"));
        size_t startAddress = 0;
        if (hasStringProperty("t")) {
            int t;
            getStringProperty("t") >> t;
            startAddress = t;
        }
        
        compileBinary(in, out, logger, startAddress);
        out.destroy();
        in.destroy();
        tmpElement->destroy();
        tmpString.destroy();
        tmpMime.destroy();
        tmp.destroy();
        
        if (logger.has(log_error)) { return 2; }
        logger.destroy();
        return 0;
    }
    
    void compileClasses(ClassPathStorage &classPath, OStream &out, Logger &logger) {
        PoolStorage &outPS = env().create<OStreamPoolStorage, OStream &>(out);
        TypeManager &types = env().create<TypeManager>();
        ClassLoader &loader = env().create<ClassLoader, ClassPathStorage&, TypeManager&>(classPath, types);
        loader.setLogger(logger);
        
        Visitor &resolveMethods = env().create<MethodResolver>();
        resolveMethods.setLogger(logger);
        Visitor &resolveTypes = env().create<TypeResolver>();
        resolveTypes.setLogger(logger);
        Visitor &generatePIR = env().create<PIRGenerator, TypeManager&>(types);
        generatePIR.setLogger(logger);
        Visitor &generatePASM = env().create<X86Writer, PoolStorage &, bool>(outPS, true);
        generatePASM.setLogger(logger);
        
        {
            Iterator<String> *it = &arguments();
            while (it->hasNext()) {
                String &name = it->next();
                ClassDeclNode * classDef = loader.getClass(name);
                if (!classDef || logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
            }
            it->destroy();
        }
        {
            Iterator<String> *it = &loader.classNames();
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
                    if (logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                    classDef->accept(generatePASM);
                    if (logger.has(log_error)) { env().err() << name << ": failed\n"; break; }
                }
            }
            it->destroy();
        }
        generatePASM.destroy();
        generatePIR.destroy();
        resolveTypes.destroy();
        resolveMethods.destroy();
        loader.destroy();
        types.destroy();
        outPS.destroy();
    }
    
    void compileBinary(IStream &in, OStream &out, Logger &logger, size_t startAddress = 0) {
        PasmParser &parser = env().create<PasmParser>();
        ASMInstructionList &list = parser.parse(in, logger.error());
        parser.destroy();
        
        if (list.hasErrors()) { return; }
        
        list.compile();
        if (list.hasErrors()) { return; }
        
        list.finalize(startAddress);
        if (list.hasErrors()) { return; }
        
        list.writeToStream(out);
        
        list.destroy();
    }
};

int main(int argc, char* argv[]) {
    PoolBootstrapStaticCompilerCommand command(LinuxBootstrap::buildEnvironment());
    return command.run(argc, argv);
}
