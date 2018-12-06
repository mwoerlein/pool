#include "poolc/ast/visitors/X86PasmVisitor.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/MethodRefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"
#include "poolc/ast/nodes/CStringConstDefNode.hpp"
#include "poolc/ast/nodes/instruction/InlinePasmInstructionNode.hpp"

// public
X86PasmVisitor::X86PasmVisitor(Environment &env, MemoryInfo &mi, OStream &out)
        :Object(env, mi), out(out),
         clsPrefix(env.create<String>()), tplPrefix(env.create<String>()),
         curClass(0), curSuper(0) {}
X86PasmVisitor::~X86PasmVisitor() {
    clsPrefix.destroy();
    tplPrefix.destroy();
}

bool X86PasmVisitor::visit(ClassRefNode & classRef) {
    out << "// class-ref " << classRef.name << "\n";
    return true;
}

bool X86PasmVisitor::visit(ClassDefNode & classDef) {
    curClass = &classDef;
    (clsPrefix = "") << "class_" << curClass->name;
    (tplPrefix = "") << clsPrefix << "_inst_tpl";
    
    out << "/*[meta]\n";
    out << "mimetype = text/x-pasm\n";
    out << "description = class \"" << curClass->fullQualifiedName << "\"\n";
    out << "[pool]\n";
    out << "version = 0.1.0\n";
    out << "class = true\n";
    out << "*/\n";
    
    // header
    out << "// class " << curClass->name << "\n";
    out << clsPrefix << "_desc:\n";
    out << "    .long 0\n";
    out << "    .long " << clsPrefix << "_so_cn_" << curClass->name << "\n";
    out << "    .long (" << tplPrefix << "_end - " << tplPrefix << ")\n"; // instance size
    out << "    .long (" << tplPrefix << " - " << clsPrefix << "_desc)\n"; // instance template offset
    out << "    .long (" << tplPrefix << "_handle_Object - " << tplPrefix << ")\n"; // Object handle offset in instance
    out << "    .long (" << tplPrefix << "_handle_" << curClass->name << " - " << tplPrefix << ")\n"; // <class> handle offset in instance
    out << "    .long 0x15AC1A55\n";
    out << "\n";

    // dependent classes
    out << clsPrefix << "_vtabs:\n";
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            out << clsPrefix << "_vtabs_entry_" << curSuper->name << ":\n";
            out << "    .long 0\n"; // @class-desc filled on class loading
            out << "    .long " << clsPrefix << "_so_cn_" << curSuper->name << "\n";
            out << "    .long (" << clsPrefix << "_vtab_" << curSuper->name << " - " << clsPrefix << "_desc)\n"; // vtab offset in description
            out << "    .long (" << tplPrefix << "_handle_" << curSuper->name << " - " << tplPrefix << ")\n"; // handle offset in instance
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
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            out << "_c" << curClass->name << "VE" << curSuper->name << " := (" << clsPrefix << "_vtabs_entry_" << curSuper->name << " - " << clsPrefix << "_desc)\n";
        }
        it.destroy();
    }
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            out << clsPrefix << "_vtab_" << curSuper->name << ":\n";
            curSuper->methodRefs.acceptAll(*this);
        }
        it.destroy();
    }
    out << "\n";
    
    // constants
    curClass->consts.acceptAll(*this);
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            out << clsPrefix << "_so_cn_" << curSuper->name << " := (" << clsPrefix << "_scn_" << curSuper->name << " - " << clsPrefix << "_desc)\n";
            out << clsPrefix << "_scn_" << curSuper->name << ":\n";
            out << "    .asciz \"" << curSuper->fullQualifiedName << "\"\n";
            out << "\n";
        }
        it.destroy();
    }
    
    // instance template and variables
    out << tplPrefix << ":\n";
    out << "    .long 0\n"; // @class-desc
    out << "    .long 0\n"; // @meminfo

    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            out << tplPrefix << "_handle_" << curSuper->name << ":\n";
            out << "    .long 0\n"; // _call_entry
            out << "    .long 0\n"; // @inst
            out << "    .long 0\n"; // vtab-offset
            Iterator<ClassDefNode> &sit = curSuper->supers.iterator();
            while (sit.hasNext()) {
                ClassDefNode & ssuper = sit.next();
                if (curSuper->equals(classDef)) {
                    out << "handle_" << curSuper->name << "_vars_" << ssuper.name << " := (" << tplPrefix << "_handle_" << curSuper->name << "_vars_" << ssuper.name << " - " << tplPrefix << "_handle_" << curSuper->name << ")\n";
                }
                out << tplPrefix << "_handle_" << curSuper->name << "_vars_" << ssuper.name << ":\n";
                out << "    .long (" << tplPrefix << "_vars_" << ssuper.name << " - " << tplPrefix << ")\n"; // @Super-Obj-Vars
            }
            sit.destroy();
        }
        it.destroy();
    }
    
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            out << tplPrefix << "_vars_" << curSuper->name << ":\n";
            curSuper->variables.acceptAll(*this);
        }
        it.destroy();
    }
    
    out << tplPrefix << "_end:\n";
    out << "\n";
    
    // methods
    curClass->methods.acceptAll(*this);
    
    return true;
}

bool X86PasmVisitor::visit(MethodRefNode & methodRef) {
    MethodDefNode & methodDef = curClass->methodRefs.get(methodRef.methodDef.name).methodDef;
    if (curSuper->equals(*curClass)) {
        out << ".global " << curSuper->name << "_m_" << methodDef.name
            << " := (" 
            << clsPrefix << "_vtab_" << curSuper->name << "_method_" << methodDef.name
            << " - " 
            << clsPrefix << "_vtab_" << curSuper->name
            << ")\n";
        out << clsPrefix << "_vtab_" << curSuper->name << "_method_" << methodDef.name << ":\n";
    }
    ClassDefNode * declClass = methodDef.parent;
    out << "    .long class_" << declClass->name << "_mo_" << methodDef.name << "\n";
    out << "    .long _c" << curClass->name << "VE" << declClass->name << "\n";
    return true;
}

bool X86PasmVisitor::visit(MethodDefNode & methodDef) {
    out << "// method-def " << methodDef.name << "\n";
    
    if (methodDef.virt) {
        out << ".global " << clsPrefix << "_mo_" << methodDef.name << " := 0 // virtual method\n";
    } else {        
        out << ".global " << clsPrefix << "_mo_" << methodDef.name
            << " := ("
            << clsPrefix << "_method_" << methodDef.name
            << " - "
            << clsPrefix << "_desc"
            << ")\n";
        
        out << clsPrefix << "_method_" << methodDef.name << ":\n";
        out << "    pushl %ebp; movl %esp, %ebp\n";
        out << "    \n";
        methodDef.body.acceptAll(*this);
        out << "    \n";
        out << "    leave\n";
        out << "    ret\n";
    }
    out << "\n";
    return true;
}

bool X86PasmVisitor::visit(VariableDefNode & variableDef) {
    out << "// variable " << variableDef.name << "\n";
    if (curSuper->equals(*curClass)) {
        out << ".global " << curSuper->name << "_i_" << variableDef.name << " := (" << tplPrefix << "_vars_" << curSuper->name << "_" << variableDef.name << " - " << tplPrefix << "_vars_" << curSuper->name << ")\n";
        out << tplPrefix << "_vars_" << curSuper->name << "_" << variableDef.name << ":\n";
    }
    out << "    .long 0\n"; // TODO: size_of variable
    return true;
}

bool X86PasmVisitor::visit(CStringConstDefNode & constDef) {
    out << "// string-const " << constDef.name << "\n";
    out << clsPrefix << "_so_ct_" << constDef.name << " := (" << clsPrefix << "_sct_" << constDef.name << " - " << clsPrefix << "_desc)\n";
    out << clsPrefix << "_sct_" << constDef.name << ":\n";
    out << "    .asciz \"" << constDef.value << "\"\n";
    out << "\n";
    return true;
}

bool X86PasmVisitor::visit(InlinePasmInstructionNode & pasmInstruction) {
    out << pasmInstruction.pasm;
    return true;
}
