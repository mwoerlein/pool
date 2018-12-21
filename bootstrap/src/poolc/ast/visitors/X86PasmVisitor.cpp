#include "poolc/ast/visitors/X86PasmVisitor.hpp"

#include "poolc/storage/Types.hpp"

#include "poolc/ast/nodes/TranslationUnitNode.hpp"
#include "poolc/ast/nodes/NamespaceDefNode.hpp"
#include "poolc/ast/nodes/UseStatementNode.hpp"
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
#define methodRefOffset(cls, m) manualClsPrefix(cls) << "_m_" << (m)->name

#define instanceStart() localClsPrefix(curClass) << "_tpl"
#define instanceEnd() localClsPrefix(curClass) << "_tpl_end"
#define instanceHandle(cls) localClsPrefix(curClass) << "_tpl_h" << localClsPrefix(cls)
#define instanceHandleVars(cls) localClsPrefix(curClass) << "_tpl_hv" << localClsPrefix(cls)
#define instanceHandleVarsOffset(cls) manualClsPrefix(curClass) << "_hvo" << manualClsPrefix(cls)
#define instanceVars(cls) localClsPrefix(curClass) << "_tpl_vs" << localClsPrefix(cls)
#define instanceVar(cls, var) localClsPrefix(curClass) << "_tpl_v" << localClsPrefix(cls) << "_" << (var)->name
#define instanceVarOffset(cls, var) manualClsPrefix(cls) << "_i_" << (var)->name

#define gMethodDeclOffset(m) globalClsPrefix((m)->parent) << "_mdo_" << (m)->name

#define OFFSET(start, end) "(" << end << " - " << start << ")"
#define CLASS_OFFSET(end) "(" << end << " - " << classDesc() << ")"
#define INSTANCE_OFFSET(end) "(" << end << " - " << instanceStart() << ")"
#define LABEL(l) *curOut << l << ":\n";
#define LONG(l) *curOut << "    .long " << l << "\n";
#define ASCIZ(str) {(str).escapeToStream(*curOut << "    .asciz "); *curOut << "\n";}
#define LOCAL(l,v) *curOut << l << " := " << v << "\n";
#define GLOBAL(l,v) *curOut << ".global " << l << " := " << v << "\n";

// public
X86PasmVisitor::X86PasmVisitor(Environment &env, MemoryInfo &mi, PoolStorage &ps)
        :Object(env, mi), ps(ps),
         mime(env.create<String, const char*>(MIMETYPE_PASM)),
         curOut(0), curClass(0), curSuper(0) {}
X86PasmVisitor::~X86PasmVisitor() {
    mime.destroy();
}

bool X86PasmVisitor::visit(TranslationUnitNode & translationUnit) {
    translationUnit.classes.acceptAll(*this);
    return true;
}

bool X86PasmVisitor::visit(NamespaceDefNode & namespaceDef) {
    return true;
}

bool X86PasmVisitor::visit(UseStatementNode & useStmt) {
    return true;
}

bool X86PasmVisitor::visit(ClassDefNode & classDef) {
    curClass = &classDef;
    curOut = &ps.writeElement(curClass->fullQualifiedName, mime);
    if (!curOut) {
        return false;
    }
    
    StorageElement &e = curClass->unit->element;
    
    *curOut << "/*[meta]\n";
    *curOut << "mimetype = " << MIMETYPE_PASM << "\n";
    *curOut << "description = class \"" << curClass->fullQualifiedName << "\"\n";
    *curOut << "[pool]\n";
    *curOut << "class = true\n";
    if (e.hasStringProperty("meta.version")) {
        *curOut << "version = " << e.getStringProperty("meta.version") << "\n";
    }
    if (e.hasStringProperty("pool.bootstrap")) {
        MethodDefNode &bs = curClass->methodRefs.get(e.getStringProperty("pool.bootstrap")).methodDef;
        if (bs.scope != scope_class || bs.kind != normal) {
            env().err() << curClass->fullQualifiedName << ": bootstrap method has to be in class scope and accessable via pool-ABI.\n";
            return false;
        }
        *curOut << "bootstrapOffset = " << gMethodDeclOffset(&bs) << "\n";
    }
    *curOut << "[pool_source]\n";
    *curOut << "unit = " << curClass->unit->name << "\n";
    if (e.hasStringProperty("meta.version")) {
        *curOut << "version = " << e.getStringProperty("meta.version") << "\n";
    }
    if (e.hasStringProperty("meta.author")) {
        *curOut << "author = " << e.getStringProperty("meta.author") << "\n";
    }
    *curOut << "*/\n";
    
    // header
    *curOut << "// class " << curClass->name << "\n";
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
    *curOut << "\n// class tab\n";
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
    *curOut << "// class tab end\n";
    LONG("0");
    LONG("0");
    LONG("0");
    LONG("0");

    // vtabs
    *curOut << "\n// method tabs\n";
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
    *curOut << "\n// constants";
    curClass->consts.acceptAll(*this);
    curClass->intConsts.acceptAll(*this);
    {
        Iterator<ClassDefNode> &it = curClass->supers.iterator();
        while (it.hasNext()) {
            curSuper = &it.next();
            *curOut << "\n// class-name " << curSuper->name << "\n";
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
    *curOut << "\n// instance template\n";
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
    *curOut << "\n// method definitions";
    curClass->methods.acceptAll(*this);
    
    curOut->destroy();
    curOut = 0;
    
    return true;
}

bool X86PasmVisitor::visit(ClassRefNode & classRef) {
    *curOut << "// class-ref " << classRef.name << "\n";
    return true;
}

bool X86PasmVisitor::visit(MethodRefNode & methodRef) {
    MethodDefNode & methodDef = curClass->methodRefs.get(methodRef.methodDef.name).methodDef;
// TODO #3: inline method-indices in method-call-generation
    LOCAL(methodRefOffset(curSuper, &methodDef), 8 * methodRef.index);
    LONG(gMethodDeclOffset(&methodDef));
    LONG(classTabOffset(methodDef.parent));
    return true;
}

bool X86PasmVisitor::visit(MethodDefNode & methodDef) {
    *curOut << "\n// method " << methodDef.name << "\n";
    
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
            
            *curOut << "    pushl %ebp; movl %esp, %ebp\n";
            *curOut << "    \n";
            methodDef.body.acceptAll(*this);
            *curOut << "    \n";
            *curOut << "    leave\n";
            *curOut << "    ret\n";
    }
    return true;
}

bool X86PasmVisitor::visit(VariableDefNode & variableDef) {
    *curOut << "// variable " << variableDef.name << "\n";
    LOCAL(
        instanceVarOffset(curSuper, &variableDef),
        OFFSET(instanceVars(curSuper), instanceVar(curSuper, &variableDef))
    );
    LABEL(instanceVar(curSuper, &variableDef));
    LONG("0"); // TODO: size_of variable
    return true;
}

bool X86PasmVisitor::visit(CStringConstDefNode & constDef) {
    *curOut << "\n// string " << constDef.name << "\n";
    LOCAL(
        constStringOffset(&constDef),
        CLASS_OFFSET(constString(&constDef))
    );
    LABEL(constString(&constDef));
    ASCIZ(constDef.value);
    return true;
}

bool X86PasmVisitor::visit(IntConstDefNode & constDef) {
    *curOut << "\n// int " << constDef.name << "\n";
    LOCAL(
        constInt(&constDef),
        constDef.value
    );
    return true;
}

bool X86PasmVisitor::visit(InlinePasmInstructionNode & pasmInstruction) {
    *curOut << pasmInstruction.pasm << "\n";
    return true;
}
