#ifndef POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK
#define POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

class X86PasmVisitor: public Visitor {
    private:
    OStream & out;
    String &clsPrefix;
    String &tplPrefix;
    ClassDefNode * curClass;
    
    public:
    X86PasmVisitor(Environment &env, MemoryInfo &mi, OStream &out)
            :Object(env, mi), out(out), clsPrefix(env.create<String>()), tplPrefix(env.create<String>()) {}
    virtual ~X86PasmVisitor() {
        clsPrefix.destroy();
        tplPrefix.destroy();
    }
    
    virtual bool visit(ClassDefNode & classDef) {
        curClass = &classDef;
        (clsPrefix = "") << "class_" << classDef.name;
        (tplPrefix = "") << clsPrefix << "_inst_tpl";
        
        out << "/*[meta]\n";
        out << "mimetype = text/x-pasm\n";
        out << "description = class \"" << classDef.fullQualifiedName << "\"\n";
        out << "[pool]\n";
        out << "version = 0.1.0\n";
        out << "class = true\n";
        out << "*/\n";
        
        // header
        out << "// class " << classDef.name << "\n";
        out << clsPrefix << "_desc:\n";
        out << "    .long 0\n";
        out << "    .long " << clsPrefix << "_so_classname\n";
        out << "    .long (" << tplPrefix << "_end - " << tplPrefix << ")\n"; // instance size
        out << "    .long (" << tplPrefix << " - " << clsPrefix << "_desc)\n"; // instance template offset
        out << "    .long (" << tplPrefix << "_handle_Object - " << tplPrefix << ")\n"; // Object handle offset in instance
        out << "    .long (" << tplPrefix << "_handle_" << classDef.name << " - " << tplPrefix << ")\n"; // <class> handle offset in instance
        out << "    .long 0x15AC1A55\n";
        out << "\n";

        // dependent classes
        out << clsPrefix << "_vtabs:\n";
        out << clsPrefix << "_vtabs_entry_Object:\n";
        out << "    .long 0\n"; // @class-desc filled on class loading
        out << "    .long " << clsPrefix << "_so_classname\n";
        out << "    .long (" << clsPrefix << "_vtab_Object - " << clsPrefix << "_desc)\n"; // vtab offset in description
        out << "    .long (" << tplPrefix << "_handle_Object - " << tplPrefix << ")\n"; // handle offset in instance
        out << clsPrefix << "_vtab_end_entry:\n";
        out << "    .long 0\n";
        out << "    .long 0\n";
        out << "    .long 0\n";
        out << "    .long 0\n";
        out << "\n";

        // vtabs
        out << "_cObjectVEObject := (" << clsPrefix << "_vtabs_entry_Object - " << clsPrefix << "_desc)\n";
        out << clsPrefix << "_vtab_Object:\n";
        {
            Iterator<MethodDefNode> &it = classDef.methods.iterator();
            while (it.hasNext()) {
                MethodDefNode & methodDef = it.next();
                out << clsPrefix << "_vtab_Object_method_" << methodDef.name << ":\n";
                out << "    .long " << clsPrefix << "_mo_" << methodDef.name << "\n";
                out << "    .long _cObjectVEObject\n";
            }
            it.destroy();
        }
        out << "\n";
        
        // constants
        out << clsPrefix << "_so_classname := (" << clsPrefix << "_string_classname - " << clsPrefix << "_desc)\n";
        out << clsPrefix << "_string_classname:\n";
        out << "    .asciz \"" << classDef.fullQualifiedName << "\"\n";
        out << "\n";
        
        // instance template and variables
        out << tplPrefix << ":\n";
        out << "    .long 0\n"; // @class-desc
        out << "    .long 0\n"; // @meminfo
        
        out << "handle_Object_vars_Object := (" << tplPrefix << "_handle_Object_vars_Object - " << tplPrefix << "_handle_Object)\n";
        out << tplPrefix << "_handle_Object:\n";
        out << "    .long 0\n"; // _call_entry
        out << "    .long 0\n"; // @inst
        out << "    .long 0\n"; // vtab-offset
        out << tplPrefix << "_handle_Object_vars_Object:\n";
        out << "    .long (" << tplPrefix << "_vars_Object - " << tplPrefix << ")\n"; // @Super-Obj-Vars
        
        out << tplPrefix << "_vars_Object:\n";
        out << ".global Object_i_runtime := (" << tplPrefix << "_vars_Object_runtime - " << tplPrefix << "_vars_Object)\n";
        out << tplPrefix << "_vars_Object_runtime:\n";
        out << "    .long 0\n"; // Runtime-handle
        
        out << tplPrefix << "_end:\n";
        out << "\n";
        
        // methods
        {
            Iterator<MethodDefNode> &it = classDef.methods.iterator();
            while (it.hasNext()) {
                it.next().accept(*this);
            }
            it.destroy();
        }
        return true;
    }
    
    virtual bool visit(MethodDefNode & methodDef) {
        out << "// method " << methodDef.name << "\n";
        
        out << ".global " << curClass->name << "_m_" << methodDef.name
            << " := (" 
            << clsPrefix << "_vtab_" << curClass->name << "_method_" << methodDef.name
            << " - " 
            << clsPrefix << "_vtab_" << curClass->name
            << ")\n";
        
        out << ".global " << clsPrefix << "_mo_" << methodDef.name
            << " := ("
            << clsPrefix << "_method_" << methodDef.name
            << " - "
            << clsPrefix << "_desc"
            << ")\n";
        
        out << clsPrefix << "_method_" << methodDef.name << ":\n";
        out << "    pushl %ebp; movl %esp, %ebp;\n";
        out << "    \n";
        out << methodDef.body;
        out << "    \n";
        out << "    leave\n";
        out << "    ret\n";
        out << "\n";
        return true;
    }
    
    virtual bool visit(VariableDefNode & variableDef) {
        out << "// variable " << variableDef.name << "\n";
    }
};

#endif //POOLC_AST_VISITORS_X86PASMVISITOR_HPP_LOCK

