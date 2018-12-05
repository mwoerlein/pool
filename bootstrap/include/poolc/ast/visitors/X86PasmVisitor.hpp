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
        out << "    .long " << clsPrefix << "_so_cn_" << classDef.name << "\n";
        out << "    .long (" << tplPrefix << "_end - " << tplPrefix << ")\n"; // instance size
        out << "    .long (" << tplPrefix << " - " << clsPrefix << "_desc)\n"; // instance template offset
        out << "    .long (" << tplPrefix << "_handle_Object - " << tplPrefix << ")\n"; // Object handle offset in instance
        out << "    .long (" << tplPrefix << "_handle_" << classDef.name << " - " << tplPrefix << ")\n"; // <class> handle offset in instance
        out << "    .long 0x15AC1A55\n";
        out << "\n";

        // dependent classes
        out << clsPrefix << "_vtabs:\n";
        {
            Iterator<ClassDefNode> &it = classDef.supers.iterator();
            while (it.hasNext()) {
                ClassDefNode & super = it.next();
                out << clsPrefix << "_vtabs_entry_" << super.name << ":\n";
                out << "    .long 0\n"; // @class-desc filled on class loading
                out << "    .long " << clsPrefix << "_so_cn_" << super.name << "\n";
                out << "    .long (" << clsPrefix << "_vtab_" << super.name << " - " << clsPrefix << "_desc)\n"; // vtab offset in description
                out << "    .long (" << tplPrefix << "_handle_" << super.name << " - " << tplPrefix << ")\n"; // handle offset in instance
            }
            it.destroy();
        }
        out << clsPrefix << "_vtab_end_entry:\n";
        out << "    .long 0\n";
        out << "    .long 0\n";
        out << "    .long 0\n";
        out << "    .long 0\n";
        out << "\n";

        // vtabs
        {
            Iterator<ClassDefNode> &it = classDef.supers.iterator();
            while (it.hasNext()) {
                ClassDefNode & super = it.next();
                out << "_c" << classDef.name << "VE" << super.name << " := (" << clsPrefix << "_vtabs_entry_" << super.name << " - " << clsPrefix << "_desc)\n";
            }
            it.destroy();
        }
        {
            Iterator<ClassDefNode> &it = classDef.supers.iterator();
            while (it.hasNext()) {
                ClassDefNode & super = it.next();
                out << clsPrefix << "_vtab_" << super.name << ":\n";
                {
                    Iterator<MethodDefNode> &it = super.methods.iterator();
                    while (it.hasNext()) {
                        MethodDefNode & methodDef = it.next();
                        if (super.equals(classDef)) {
                            out << clsPrefix << "_vtab_" << super.name << "_method_" << methodDef.name << ":\n";
                        }
                        ClassDefNode & declClass = super;
                        out << "    .long class_" << declClass.name << "_mo_" << methodDef.name << "\n";
                        out << "    .long _c" << classDef.name << "VE" << declClass.name << "\n";
                    }
                    it.destroy();
                }
            }
            it.destroy();
        }
        out << "\n";
        
        // constants
        {
            Iterator<ClassDefNode> &it = classDef.supers.iterator();
            while (it.hasNext()) {
                ClassDefNode & super = it.next();
                out << clsPrefix << "_so_cn_" << super.name << " := (" << clsPrefix << "_string_cn_" << super.name << " - " << clsPrefix << "_desc)\n";
                out << clsPrefix << "_string_cn_" << super.name << ":\n";
                out << "    .asciz \"" << super.fullQualifiedName << "\"\n";
                out << "\n";
            }
            it.destroy();
        }
        
        // instance template and variables
        out << tplPrefix << ":\n";
        out << "    .long 0\n"; // @class-desc
        out << "    .long 0\n"; // @meminfo

        {
            Iterator<ClassDefNode> &it = classDef.supers.iterator();
            while (it.hasNext()) {
                ClassDefNode & super = it.next();
                
                out << tplPrefix << "_handle_" << super.name << ":\n";
                out << "    .long 0\n"; // _call_entry
                out << "    .long 0\n"; // @inst
                out << "    .long 0\n"; // vtab-offset
                Iterator<ClassDefNode> &sit = super.supers.iterator();
                while (sit.hasNext()) {
                    ClassDefNode & ssuper = sit.next();
                    if (super.equals(classDef)) {
                        out << "handle_" << super.name << "_vars_" << ssuper.name << " := (" << tplPrefix << "_handle_" << super.name << "_vars_" << ssuper.name << " - " << tplPrefix << "_handle_" << super.name << ")\n";
                    }
                    out << tplPrefix << "_handle_" << super.name << "_vars_" << ssuper.name << ":\n";
                    out << "    .long (" << tplPrefix << "_vars_" << ssuper.name << " - " << tplPrefix << ")\n"; // @Super-Obj-Vars
                }
                sit.destroy();
            }
            it.destroy();
        }
        
        {
            Iterator<ClassDefNode> &it = classDef.supers.iterator();
            while (it.hasNext()) {
                ClassDefNode & super = it.next();
                out << tplPrefix << "_vars_" << super.name << ":\n";
                Iterator<VariableDefNode> &vit = super.variables.iterator();
                while (vit.hasNext()) {
                    VariableDefNode &variable = vit.next();
                    if (super.equals(classDef)) {
                        out << ".global " << super.name << "_i_" << variable.name << " := (" << tplPrefix << "_vars_" << super.name << "_" << variable.name << " - " << tplPrefix << "_vars_" << super.name << ")\n";
                        out << tplPrefix << "_vars_" << super.name << "_" << variable.name << ":\n";
                    }
                    out << "    .long 0 // " << variable.name << "\n"; // TODO: size_of variable
                }
                vit.destroy();
            }
            it.destroy();
        }
        
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
        
        out << classDef.inlinePasm << "\n";
        
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

