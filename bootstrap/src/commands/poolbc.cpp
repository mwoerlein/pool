#include "linux/LinuxBootstrap.hpp"
#include "linux/CommandLine.hpp"

#include "poolc/ast/SimpleFactory.hpp"
#include "poolc/ast/visitors/ResolveVisitor.hpp"
#include "poolc/ast/visitors/X86PasmVisitor.hpp"

static const char PROGRAM[] = "poolbc";
static const char VERSION[] = "0.1.0";
static const char USAGE[] =
R"(Pool Bootstrap Compiler.

    Usage:
      poolbc [options] -o <file> <pool-file>

    Options:
      -h --help    Show this screen.
      --version    Show version.
      -o <file>    Place the (pasm) output into <file>.
)";

class PoolBootstrapCompilerCommand: public CommandLine {
    public:
    PoolBootstrapCompilerCommand(Environment & env, MemoryInfo & mi = *notAnInfo)
            :CommandLine(env, mi), Object(env, mi) {
    }
    virtual ~PoolBootstrapCompilerCommand() {
    }
    
    int run(int argc, char* argv[]) {
        parse(argc, argv);
        
        if (hasProperty("h") || hasProperty("help")) {
            env().out()<<USAGE;
            return 0;
        }
        if (hasProperty("version")) {
            env().out()<<PROGRAM<<" "<<VERSION<<"\n";
            return 0;
        }
        if (!hasStringProperty("o") || _arguments.size() != 1) {
            env().out()<<USAGE;
            return -1;
        }
        
        SimpleFactory &f = env().create<SimpleFactory>();
        Iterator<String> & argIt = arguments();
        ClassDefNode *classDef = f.getDef(argIt.next());
        argIt.destroy();
        
        if (classDef) {
            Visitor &resolve = env().create<ResolveVisitor, SimpleFactory &>(f);
            classDef->accept(resolve);
            resolve.destroy();
            
            SeekableIOStream &outfile = env().streamFactory().buildOStream(getStringProperty("o"));
            Visitor &dump = env().create<X86PasmVisitor, OStream &>(outfile);
            classDef->accept(dump);
            dump.destroy();
            outfile.destroy();
        }

        f.destroy();
    }
};

int main(int argc, char* argv[]) {
    PoolBootstrapCompilerCommand command(LinuxBootstrap::buildEnvironment());
    return command.run(argc, argv);
}
