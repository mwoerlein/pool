#include "poolc/ast/visitors/X86PasmVisitor.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/MethodRefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"
#include "poolc/ast/nodes/CStringConstDefNode.hpp"
#include "poolc/ast/nodes/instruction/InlinePasmInstructionNode.hpp"

// TODO: cleanup/shorten prefixes/ids
#define clsPrefix(cls) "class_" << (cls)->name
#define clsPrefix2(cls) "_c" << (cls)->name
#define clsPrefix3(cls) (cls)->name
#define tplPrefix(cls) clsPrefix(cls) << "_inst_tpl"
#define tplPrefix2(cls) "handle_" << (cls)->name

#define classDesc() clsPrefix(curClass) << "_desc"
#define classTab(cls) clsPrefix(curClass) << "_vtabs_entry_" << (cls)->name
#define classTabOffset(cls) clsPrefix2(curClass) << "VE" << (cls)->name
#define className(cls) clsPrefix(curClass) << "_scn_" << (cls)->name
#define classNameOffset(cls) clsPrefix(curClass) << "_so_cn_" << (cls)->name
#define constString(c) clsPrefix(curClass) << "_sct_" << (c)->name
#define constStringOffset(c) clsPrefix(curClass) << "_so_ct_" << (c)->name
#define methodDecl(m) clsPrefix((m)->parent) << "_method_" << (m)->name
#define methodDeclOffset(m) clsPrefix((m)->parent) << "_mo_" << (m)->name
#define methodTab(cls) clsPrefix(curClass) << "_vtab_" << (cls)->name
#define methodRef(cls, m) clsPrefix(curClass) << "_vtab_" << (cls)->name << "_method_" << (m)->name
#define methodRefOffset(cls, m) clsPrefix3(cls) << "_m_" << (m)->name

#define instanceStart() tplPrefix(curClass)
#define instanceEnd() tplPrefix(curClass) << "_end"
#define instanceHandle(cls) tplPrefix(curClass) << "_handle_" << (cls)->name
#define instanceHandleVars(cls, cls2) tplPrefix(curClass) << "_handle_" << (cls)->name << "_vars_" << (cls2)->name
#define instanceVars(cls) tplPrefix(curClass) << "_vars_" << (cls)->name
#define instanceVarsOffset(cls) tplPrefix2(curClass) << "_vars_" << (cls)->name
#define instanceVar(cls, var) tplPrefix(curClass) << "_vars_" << (cls)->name << "_" << (var)->name
#define instanceVarOffset(cls, var) clsPrefix3(cls) << "_i_" << (var)->name

#define OFFSET(start, end) "(" << end << " - " << start << ")"
#define CLASS_OFFSET(end) "(" << end << " - " << classDesc() << ")"
#define INSTANCE_OFFSET(end) "(" << end << " - " << instanceStart() << ")"
#define LABEL(l) out << l << ":\n";
#define LONG(l) out << "    .long " << l << "\n";
#define ASCIZ(str) {(str).escapeToStream(out << "    .asciz "); out << "\n";}
#define LOCAL(l,v) out << l << " := " << v << "\n";
#define GLOBAL(l,v) out << ".global " << l << " := " << v << "\n";

// public
X86PasmVisitor::X86PasmVisitor(Environment &env, MemoryInfo &mi, OStream &out)
        :Object(env, mi), out(out),
         curClass(0), curSuper(0) {}
X86PasmVisitor::~X86PasmVisitor() {}

bool X86PasmVisitor::visit(ClassRefNode & classRef) {
    out << "// class-ref " << classRef.name << "\n";
    return true;
}

bool X86PasmVisitor::visit(ClassDefNode & classDef) {
    curClass = &classDef;
    
    out << "/*[meta]\n";
    out << "mimetype = text/x-pasm\n";
    out << "description = class \"" << curClass->fullQualifiedName << "\"\n";
    out << "[pool]\n";
    out << "version = 0.1.0\n";
    out << "class = true\n";
    if (curClass->bootstrap) {
        out << "bootstrapOffset = " << methodDeclOffset(curClass->bootstrap) << "\n";
    }
    out << "*/\n";
    
    // header
    out << "// class " << curClass->name << "\n";
    LABEL(classDesc());
    LONG("0x15AC1A55");
    LONG("0");
    LONG(classNameOffset(curClass));
    LONG(CLASS_OFFSET(instanceStart())); // instance template offset
    LONG(INSTANCE_OFFSET(instanceEnd())); // instance size
    LONG(INSTANCE_OFFSET(instanceHandle(curClass->supers.first()))); // Object handle offset in instance
    LONG(INSTANCE_OFFSET(instanceHandle(curClass))); // <class> handle offset in instance

    // dependent classes
    out << "\n// class tab\n";
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            LOCAL(
                classTabOffset(curSuper),
                CLASS_OFFSET(classTab(curSuper))
            );
            LABEL(classTab(curSuper));
            LONG("0"); // @class-desc filled on class loading
            LONG(classNameOffset(curSuper));
            LONG(CLASS_OFFSET(methodTab(curSuper))); //  vtab offset in description
            LONG(INSTANCE_OFFSET(instanceHandle(curSuper))); // handle offset in instance
        }
        it.destroy();
    }
    out << "// class tab end\n";
    LONG("0");
    LONG("0");
    LONG("0");
    LONG("0");

    // vtabs
    out << "\n// method tabs\n";
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            LABEL(methodTab(curSuper));
            curSuper->methodRefs.acceptAll(*this);
        }
        it.destroy();
    }
    
    // constants
    out << "\n// constants";
    curClass->consts.acceptAll(*this);
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            out << "\n// class-name " << curSuper->name << "\n";
            LOCAL(
                classNameOffset(curSuper),
                CLASS_OFFSET(className(curSuper))
            );
            LABEL(className(curSuper));
            ASCIZ(curSuper->fullQualifiedName);
        }
        it.destroy();
    }
    
    // instance template and variables
    out << "\n// instance template\n";
    LABEL(instanceStart());
    LONG("0"); // @class-desc
    LONG("0"); // @meminfo

    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            LABEL(instanceHandle(curSuper));
            LONG("0"); // _call_entry
            LONG("0"); // @inst
            LONG("0"); // vtab-offset
            
            Iterator<ClassDefNode> &sit = curSuper->supers.iterator();
            while (sit.hasNext()) {
                ClassDefNode * ssuper = &sit.next();
                if (curSuper->equals(*curClass)) {
                    LOCAL(
                        instanceVarsOffset(ssuper),
                        OFFSET(instanceHandle(curSuper), instanceHandleVars(curSuper, ssuper))
                    );
                }
                LABEL(instanceHandleVars(curSuper, ssuper));
                LONG(INSTANCE_OFFSET(instanceVars(ssuper))); // @Super-Obj-Vars
            }
            sit.destroy();
        }
        it.destroy();
    }
    
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            LABEL(instanceVars(curSuper));
            curSuper->variables.acceptAll(*this);
        }
        it.destroy();
    }
    
    LABEL(instanceEnd());
    
    // methods
    out << "\n// method definitions";
    curClass->methods.acceptAll(*this);
    
    return true;
}

bool X86PasmVisitor::visit(MethodRefNode & methodRef) {
    MethodDefNode & methodDef = curClass->methodRefs.get(methodRef.methodDef.name).methodDef;
    if (curSuper->equals(*curClass)) {
        GLOBAL(
            methodRefOffset(curSuper, &methodDef), 
            OFFSET(methodTab(curSuper), methodRef(curSuper, &methodDef))
        );
        LABEL(methodRef(curSuper, &methodDef));
    }
    LONG(methodDeclOffset(&methodDef));
    LONG(classTabOffset(methodDef.parent));
    return true;
}

bool X86PasmVisitor::visit(MethodDefNode & methodDef) {
    out << "\n// method " << methodDef.name << "\n";
    
    if (methodDef.virt) {
        GLOBAL(
            methodDeclOffset(&methodDef),
            "0" // virtual method
        );
    } else if (methodDef.naked) {
        methodDef.body.acceptAll(*this);
    } else {        
        GLOBAL(
            methodDeclOffset(&methodDef),
            CLASS_OFFSET(methodDecl(&methodDef))
        );
        LABEL(methodDecl(&methodDef));
        
        out << "    pushl %ebp; movl %esp, %ebp\n";
        out << "    \n";
        methodDef.body.acceptAll(*this);
        out << "    \n";
        out << "    leave\n";
        out << "    ret\n";
    }
    return true;
}

bool X86PasmVisitor::visit(VariableDefNode & variableDef) {
    out << "// variable " << variableDef.name << "\n";
    if (curSuper->equals(*curClass)) {
        GLOBAL(
            instanceVarOffset(curSuper, &variableDef),
            OFFSET(instanceVars(curSuper), instanceVar(curSuper, &variableDef))
        );
        LABEL(instanceVar(curSuper, &variableDef));
    }
    LONG("0"); // TODO: size_of variable
    return true;
}

bool X86PasmVisitor::visit(CStringConstDefNode & constDef) {
    out << "\n// string " << constDef.name << "\n";
    LOCAL(
        constStringOffset(&constDef),
        CLASS_OFFSET(constString(&constDef))
    );
    LABEL(constString(&constDef));
    ASCIZ(constDef.value);
    return true;
}

bool X86PasmVisitor::visit(InlinePasmInstructionNode & pasmInstruction) {
    out << pasmInstruction.pasm;
    return true;
}
