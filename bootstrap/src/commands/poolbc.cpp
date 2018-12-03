#include "linux/LinuxBootstrap.hpp"
#include "linux/CommandLine.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
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
    private:
    
    ClassDefNode & generateObjectDef() {
        ClassDefNode &classDef = env().create<ClassDefNode>();
        classDef.name = "Object";
        classDef.fullQualifiedName = "/my/Object";
        
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "getClass";
            method.body
                << "    movl 12(%ebp), %eax    // @this (Type Object)\n"
                << "    movl 4(%eax), %eax     // @this\n"
                << "    movl (%eax), %eax      // @class desc\n"
                << "    movl (%eax), %eax      // @class handle\n"
                << "    movl %eax, 16(%ebp)    // return @class handle\n"
            ;
            classDef.methods.add(method);
        }
        
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "hash";
            method.body
                << "    movl 12(%ebp), %eax    // @this (Type Object)\n"
                << "    movl 4(%eax), %eax     // @this\n"
                << "    movl %eax, 16(%ebp)    // return @this as hash\n"
            ;
            classDef.methods.add(method);
        }
        
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "equals";
            method.body
                << "    movl 0, 20(%ebp)       // default return: false\n"
                << "    movl 12(%ebp), %eax    // @this (Type Object)\n"
                << "    movl 4(%eax), %eax     // @this\n"
                << "    movl 16(%ebp), %ebx    // @obj (Type ANY)\n"
                << "    movl 4(%ebx), %ebx     // @obj\n"
                << "    cmpl %eax, %ebx\n"
                << "    jne _come_ret\n"
                << "    movl 1, 20(%ebp)       // return true\n"
                << "_come_ret:\n"    
            ;
            classDef.methods.add(method);
        }
        
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "rt";
            method.body
                << "    movl 12(%ebp), %eax                         // @this (Type Object)\n"
                << "    movl handle_Object_vars_Object(%eax), %ebx  // inst vars offset (Object)\n"
                << "    addl 4(%eax), %ebx                          // @this.vars(Object)\n"
                << "    movl Object_i_runtime(%ebx), %eax           // load @runtime (Type Runtime)\n"
                << "    movl %eax, 16(%ebp)                         // return @runtime (Type Runtime)\n"
            ;
            classDef.methods.add(method);
        }
        
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "setRt";
            method.body
                << "    movl 12(%ebp), %eax                         // @this (Type Object)\n"
                << "    movl handle_Object_vars_Object(%eax), %ebx  // inst vars offset (Object)\n"
                << "    addl 4(%eax), %ebx                          // @this.vars(Object)\n"
                << "    movl 16(%ebp), %eax                         // arg @runtime (Type Runtime)\n"
                << "    movl %eax, Object_i_runtime(%ebx)           // store @runtime (Type Runtime)\n"
            ;
            classDef.methods.add(method);
        }
        
        return classDef;
    }
    
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
        
        Iterator<String> & argIt = arguments();
        String &name = argIt.next();
        argIt.destroy();
        
        SeekableIOStream &outfile = env().streamFactory().buildOStream(getStringProperty("o"));
        Visitor &dump = env().create<X86PasmVisitor, OStream &>(outfile);
        
        ClassDefNode *classDef = 0;
        if (name == "Object") {
            classDef = &generateObjectDef();
        }
        
        if (classDef) {
            classDef->accept(dump);
            classDef->destroy();
        }

        outfile.destroy();
    }
};

int main(int argc, char* argv[]) {
    PoolBootstrapCompilerCommand command(LinuxBootstrap::buildEnvironment());
    return command.run(argc, argv);
}
