#include "linux/LinuxBootstrap.hpp"
#include "linux/CommandLine.hpp"

#include "pasm/i386/Parser.hpp"

static const char PROGRAM[] = "pbasm";
static const char VERSION[] = "0.2.0";
static const char USAGE[] =
R"(Pool Bootstrap Assembler.

    Usage:
      pasm [options] -o <file> <file>

    Options:
      -h --help                 Show this screen.
      --version                 Show version.
      -o <file> --output <file> Place the output into <file>.
      -b --binary               Generate plain binary without module informations.
      -c --class                Generate class module informations.
      -e --entry                Generate entry module informations.
      --bootstrap <label>       Generate bootstrap informations.
      -t <org>                  Locates code in the output file at the absolute address given by org.
      -g <file>                 Place the global definitions into <file>.
)";

class PasmCommand: public CommandLine {
    private:
    Parser &parser;
    
    public:
    PasmCommand(Environment & env, MemoryInfo & mi = *notAnInfo)
            :CommandLine(env, mi), Object(env, mi), parser(env.create<Parser>()) {
        registerOptionAlias("help", "h");
        registerOptionAlias("output", "o");
        registerOptionAlias("binary", "b");
        registerOptionAlias("class", "c");
        registerOptionAlias("entry", "e");
    }
    virtual ~PasmCommand() {
        parser.destroy();
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
        if (!hasStringProperty("output") || _arguments.size() != 1) {
            env().out()<<USAGE;
            return -1;
        }
        size_t startAddress = 0;
        if (hasStringProperty("t")) {
            int t;
            getStringProperty("t") >> t;
            startAddress = t;
        }
        
        OStream &outfile = env().streamFactory().buildOStream(getStringProperty("output"));
        Iterator<String> & argIt = arguments();
        String &name = argIt.next();
        IStream &infile = (name == "-") ? env().streamFactory().buildStdIStream() : env().streamFactory().buildIStream(name);
        argIt.destroy();
        
        ASMInstructionList &list = parser.parse(infile, env().err());
        if (list.hasErrors()) { return 1; }
        
        list.compile();
        if (list.hasErrors()) { return 1; }
        
        list.finalize(startAddress);
        if (list.hasErrors()) { return 1; }
        
        if (hasProperty("g")) {
            OStream &globalsfile = env().streamFactory().buildOStream(getStringProperty("g"));
            list.logGlobalsToStream(globalsfile);
            globalsfile.destroy();
        }

        // generate module infos
        if (!hasProperty("binary")) {
            outfile<<"/*[meta]\n";
            outfile<<"mimetype = application/x-bin-x86\n";
            
            outfile<<"[pool]\n";
            outfile<<"version = "<<VERSION<<'\n';
            if (hasProperty("class")) {
                outfile<<"class = true\n";
            }
            if (hasProperty("entry")) {
                outfile<<"entry = true\n";
            }
            if (hasProperty("bootstrap") && list.hasDefinition(getStringProperty("bootstrap"))) {
                outfile<<"bootstrapOffset = "<<list.getValue(getStringProperty("bootstrap"))<<'\n';
            }
            
            outfile<<"*/\n";
        }
        list.writeToStream(outfile);
        
        infile.destroy();
        outfile.destroy();
        list.destroy();
    }
};

int main(int argc, char* argv[]) {
    PasmCommand command(LinuxBootstrap::buildEnvironment());
    return command.run(argc, argv);
}
