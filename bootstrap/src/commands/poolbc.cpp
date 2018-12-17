#include "linux/LinuxBootstrap.hpp"
#include "linux/CommandLine.hpp"
#include "linux/DirectoryPoolStorage.hpp"

#include "poolc/storage/Types.hpp"
#include "poolc/storage/ClassPathStorage.hpp"
#include "poolc/parser/PoolParser.hpp"
#include "poolc/ast/SimpleFactory.hpp"
#include "poolc/ast/visitors/ResolveVisitor.hpp"
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
      -o <dir> --output <dir>       Place the (pasm) outputs into <dir>.
      -c <dir> --classpath <dir>    Search for classes in all of these directories.
)";

class PoolBootstrapCompilerCommand: public CommandLine {
    public:
    PoolBootstrapCompilerCommand(Environment & env, MemoryInfo & mi = *notAnInfo)
            :CommandLine(env, mi), Object(env, mi) {
        registerOptionAlias("help", "h");
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
        
        SimpleFactory &f = env().create<SimpleFactory>();
        String & mimePasm = env().create<String, const char*>(MIMETYPE_PASM);
        
        DirectoryPoolStorage &out = env().create<DirectoryPoolStorage, String&>(getStringProperty("output"));
        ClassPathStorage &cp = env().create<ClassPathStorage>();
        {
            Iterator<String> &it = optionSet("classpath");
            while (it.hasNext()) {
                cp.addStore(env().create<DirectoryPoolStorage, String&>(it.next()));
            }
            it.destroy();
        }
        
        Iterator<String> & argIt = arguments();
        while (argIt.hasNext()) {
            String &name = argIt.next();
//            env().out()<<"compile "<<name<<"\n";
            
            TranslationUnitNode *unit = 0;
            
            if (StorageElement *e = cp.getElement(name)) {
                IStream &infile = e->getContent();
                PoolParser &p = env().create<PoolParser>();
                unit = p.parse(infile, name);
                p.destroy();
                infile.destroy();
            }
            
            if (unit) {
                Visitor &resolve = env().create<ResolveVisitor, SimpleFactory &>(f);
                unit->accept(resolve);
                resolve.destroy();
                
                OStream &output = out.writeElement(name, mimePasm);
                if (&output) {
                    Visitor &dump = env().create<X86PasmVisitor, OStream &>(output);
                    unit->accept(dump);
                    dump.destroy();
                    output.destroy();
                }
                
                unit->destroy();
            }
        }
        argIt.destroy();
        
        out.destroy();
        cp.destroy();
        
        mimePasm.destroy();
        f.destroy();
        return 0;
    }
};

int main(int argc, char* argv[]) {
    PoolBootstrapCompilerCommand command(LinuxBootstrap::buildEnvironment());
    return command.run(argc, argv);
}
