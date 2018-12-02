#include "linux/LinuxBootstrap.hpp"
#include "linux/CommandLine.hpp"

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
        
        SeekableIOStream &outfile = env().streamFactory().buildOStream(getStringProperty("o"));
        outfile<<"/*[meta]\n";
        outfile<<"mimetype = text/x-pasm\n";
        outfile<<"description = class \""<<"/my/A"<<"\"\n";
        outfile<<"[pool]\n";
        outfile<<"version = "<<VERSION<<'\n';
        outfile<<"class = true\n";
        outfile<<"*/\n";

        outfile.destroy();
    }
};

int main(int argc, char* argv[]) {
    PoolBootstrapCompilerCommand command(LinuxBootstrap::buildEnvironment());
    return command.run(argc, argv);
}
