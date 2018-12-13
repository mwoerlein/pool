#include "poolc/ast/visitors/X86PasmVisitor.hpp"

#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/MethodRefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"
#include "poolc/ast/nodes/CStringConstDefNode.hpp"
#include "poolc/ast/nodes/IntConstDefNode.hpp"
#include "poolc/ast/nodes/instruction/InlinePasmInstructionNode.hpp"

#define globalClsPrefix(cls) (cls)->globalPrefix
#define localClsPrefix(cls) (cls)->localPrefix
// TODO: #3 replace with localClsPrefix after inline pasm is replaced with method-code generation
#define manualClsPrefix(cls) (cls)->globalPrefix

#define classDesc() manualClsPrefix(curClass)
#define classTabs() localClsPrefix(curClass) << "_cts"
#define classTab(cls) localClsPrefix(curClass) << "_ct" << localClsPrefix(cls)
#define classTabOffset(cls) localClsPrefix(curClass) << "_cto" << localClsPrefix(cls)
#define className(cls) localClsPrefix(curClass) << "_cn" << localClsPrefix(cls)
#define classNameOffset(cls) manualClsPrefix(curClass) << "_cno" << manualClsPrefix(cls)
#define constInt(c) manualClsPrefix(curClass) << "_coi_" << (c)->name
#define constString(c) localClsPrefix(curClass) << "_cos_" << (c)->name
#define constStringOffset(c) manualClsPrefix(curClass) << "_coso_" << (c)->name
#define methodDecl(m) localClsPrefix((m)->parent) << "_md_" << (m)->name
#define methodTabs() localClsPrefix(curClass) << "_mts"
#define methodTab(cls) localClsPrefix(curClass) << "_mt" << localClsPrefix(cls)
#define methodRef(cls, m) localClsPrefix(curClass) << "_mtm" << localClsPrefix(cls) << "_" << (m)->name

#define instanceStart() localClsPrefix(curClass) << "_tpl"
#define instanceEnd() localClsPrefix(curClass) << "_tpl_end"
#define instanceHandle(cls) localClsPrefix(curClass) << "_tpl_h" << localClsPrefix(cls)
#define instanceHandleVars(cls) localClsPrefix(curClass) << "_tpl_hv" << localClsPrefix(cls)
#define instanceVars(cls) localClsPrefix(curClass) << "_tpl_vs" << localClsPrefix(cls)
#define instanceVar(cls, var) localClsPrefix(curClass) << "_tpl_v" << localClsPrefix(cls) << "_" << (var)->name
#define instanceHandleVarsOffset(cls) manualClsPrefix(curClass) << "_hvo" << manualClsPrefix(cls)

#define gConstInt(c) globalClsPrefix(curClass) << "_c_" << (c)->name
#define gMethodDeclOffset(m) globalClsPrefix((m)->parent) << "_mdo_" << (m)->name
#define gMethodRefOffset(cls, m) globalClsPrefix(cls) << "_m_" << (m)->name
#define gInstanceVarOffset(cls, var) globalClsPrefix(cls) << "_i_" << (var)->name

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
        out << "bootstrapOffset = " << gMethodDeclOffset(curClass->bootstrap) << "\n";
    }
    out << "*/\n";
    
    // header
    out << "// class " << curClass->name << "\n";
    LABEL(classDesc());
    LONG("0x15AC1A55");
    LONG("0");
    LONG(classNameOffset(curClass));
    LONG(CLASS_OFFSET(classTabs()));      // class tabs offset
    LONG(CLASS_OFFSET(methodTabs()));     // method tabs offset
    LONG(CLASS_OFFSET(instanceStart()));  // instance template offset
    LONG(INSTANCE_OFFSET(instanceEnd())); // instance size
    LONG(INSTANCE_OFFSET(instanceHandle(curClass->supers.first()))); // Object handle offset in instance
    LONG(INSTANCE_OFFSET(instanceHandle(curClass))); // <class> handle offset in instance

    // dependent classes
    out << "\n// class tab\n";
    LABEL(classTabs());
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
    LABEL(methodTabs());
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
    curClass->intConsts.acceptAll(*this);
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
                        instanceHandleVarsOffset(ssuper),
                        OFFSET(instanceHandle(curSuper), instanceHandleVars(ssuper))
                    );
                    LABEL(instanceHandleVars(ssuper));
                }
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
            gMethodRefOffset(curSuper, &methodDef), 
            OFFSET(methodTab(curSuper), methodRef(curSuper, &methodDef))
        );
        LABEL(methodRef(curSuper, &methodDef));
    }
    LONG(gMethodDeclOffset(&methodDef));
    LONG(classTabOffset(methodDef.parent));
    return true;
}

bool X86PasmVisitor::visit(MethodDefNode & methodDef) {
    out << "\n// method " << methodDef.name << "\n";
    
    switch (methodDef.kind) {
        case abstract:
            GLOBAL(
                gMethodDeclOffset(&methodDef),
                "0" // virtual method
            );
            break;
        case naked:
            methodDef.body.acceptAll(*this);
            break;
        default:
            GLOBAL(
                gMethodDeclOffset(&methodDef),
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
            gInstanceVarOffset(curSuper, &variableDef),
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

bool X86PasmVisitor::visit(IntConstDefNode & constDef) {
    out << "\n// int " << constDef.name << "\n";
    if (constDef.global) {
        GLOBAL(
            gConstInt(&constDef),
            constDef.value
        );
    } else {
        LOCAL(
            constInt(&constDef),
            constDef.value
        );
    }
    return true;
}

bool X86PasmVisitor::visit(InlinePasmInstructionNode & pasmInstruction) {
    out << pasmInstruction.pasm << "\n";
    return true;
}
