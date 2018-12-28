#include "poolc/ast/visitors/X86PasmVisitor.hpp"

#include "poolc/storage/Types.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

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
#define methodDeclTab() localClsPrefix(curClass) << "_mdt"
#define methodDecl(m) localClsPrefix((m)->scope->getClassDeclNode()) << "_md_" << (m)->name
#define methodDeclOffset(m) manualClsPrefix((m)->scope->getClassDeclNode()) << "_mdo_" << (m)->name
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

#define OFFSET(start, end) "(" << end << " - " << start << ")"
#define CLASS_OFFSET(end) "(" << end << " - " << classDesc() << ")"
#define INSTANCE_OFFSET(end) "(" << end << " - " << instanceStart() << ")"
#define LABEL(l) elem() << l << ":\n";
#define LONG(l) elem() << "    .long " << l << "\n";
#define ASCIZ(str) {(str).escapeToStream(elem() << "    .asciz "); elem() << "\n";}
#define LOCAL(l,v) elem() << l << " := " << v << "\n";

// public
X86PasmVisitor::X86PasmVisitor(Environment &env, MemoryInfo &mi, PoolStorage &ps)
        :Writer(env, mi, ps, MIMETYPE_PASM), Object(env, mi), LoggerAware(env, mi),
         curClass(0), curSuper(0) {}
X86PasmVisitor::~X86PasmVisitor() {}

bool X86PasmVisitor::visit(TranslationUnitNode & translationUnit) {
    translationUnit.classes.acceptAll(*this);
    return true;
}

bool X86PasmVisitor::visit(ClassDeclNode & classDef) {
    curClass = &classDef;
    if (!startElement(curClass->fullQualifiedName)) {
        return false;
    }
    
    ClassScope *classScope = curClass->scope->isClass();
    InstanceScope *instanceScope = curClass->instanceScope;
    TranslationUnitNode *unit = classScope->getUnitNode();
    StorageElement &e = unit->element;
    
    elem() << "/*[meta]\n";
    elem() << "mimetype = " << MIMETYPE_PASM << "\n";
    elem() << "description = class \"" << curClass->fullQualifiedName << "\"\n";
    elem() << "[pool]\n";
    elem() << "class = true\n";
    if (e.hasStringProperty("meta.version")) {
        elem() << "version = " << e.getStringProperty("meta.version") << "\n";
    }
    if (e.hasStringProperty("pool.bootstrap")) {
        MethodDeclNode *bs = classScope->getMethod(e.getStringProperty("pool.bootstrap"))->getMethodDeclNode();
        if (!bs || bs->kind != normal) {
            error() << curClass->fullQualifiedName << ": bootstrap method has to be iaccessible via pool-ABI.\n";
            return false;
        }
        elem() << "bootstrapOffset = " << methodDeclOffset(bs) << "\n";
    }
    elem() << "[pool_source]\n";
    elem() << "unit = " << unit->name << "\n";
    if (e.hasStringProperty("meta.version")) {
        elem() << "version = " << e.getStringProperty("meta.version") << "\n";
    }
    if (e.hasStringProperty("meta.author")) {
        elem() << "author = " << e.getStringProperty("meta.author") << "\n";
    }
    elem() << "*/\n";
    
    // header
    elem() << "// class " << curClass->name << "\n";
    LABEL(classDesc());
    LONG("0x15AC1A55");
    LONG("0");
    LONG(classNameOffset(curClass));
    LONG(CLASS_OFFSET(classTabs()));      // class tabs offset
    LONG(CLASS_OFFSET(methodTabs()));     // method tabs offset
    LONG(CLASS_OFFSET(methodDeclTab()));  // methods tab offset
    LONG(CLASS_OFFSET(instanceStart()));  // instance template offset
    LONG(INSTANCE_OFFSET(instanceEnd())); // instance size
    LONG(INSTANCE_OFFSET(instanceHandle(classScope->firstSuper()->getClassDeclNode()))); // Object handle offset in instance
    LONG(INSTANCE_OFFSET(instanceHandle(curClass))); // <class> handle offset in instance

    // dependent classes
    elem() << "\n// class tab\n";
    LABEL(classTabs());
    {
        Iterator<ClassScope> &it = classScope->supers();
        while (it.hasNext()) {
            ClassDeclNode *superDecl = it.next().getClassDeclNode();
            LOCAL(
                classTabOffset(superDecl),
                CLASS_OFFSET(classTab(superDecl))
            );
            LABEL(classTab(superDecl));
            LONG("0"); // @class-desc filled on class loading
            LONG(classNameOffset(superDecl));
            LONG(CLASS_OFFSET(methodTab(superDecl))); //  vtab offset in description
            LONG(INSTANCE_OFFSET(instanceHandle(superDecl))); // handle offset in instance
        }
        it.destroy();
    }
    elem() << "// class tab end\n";
    LONG("0");
    LONG("0");
    LONG("0");
    LONG("0");

    // vtabs
    elem() << "\n// method tabs\n";
    LABEL(methodTabs());
    {
        Iterator<ClassScope> &it = classScope->supers();
        while (it.hasNext()) {
            ClassScope &superClassScope = it.next();
            ClassDeclNode *superClassDecl = superClassScope.getClassDeclNode();
            InstanceScope *superInstanceScope = superClassDecl->instanceScope;
            
            LABEL(methodTab(superClassDecl));
            {
                Iterator<MethodScope> &mit = superInstanceScope->methods();
                while (mit.hasNext()) {
                    MethodScope & superMethodScope = mit.next();
                    MethodDeclNode * methodDecl = instanceScope->getMethod(superMethodScope)->getMethodDeclNode();
                    if (methodDecl->kind == naked) { continue; }
                    // TODO #3: inline method-indices in method-call-generation
                    LOCAL(methodRefOffset(superClassDecl, methodDecl), 8 * superMethodScope.index);
                    LONG(4 * methodDecl->index);
                    LONG(classTabOffset(methodDecl->scope->getClassDeclNode()));
                }
                mit.destroy();
            }
            {
                Iterator<MethodScope> &mit = superClassScope.methods();
                while (mit.hasNext()) {
                    MethodScope & superMethodScope = mit.next();
                    MethodDeclNode * methodDecl = classScope->getMethod(superMethodScope)->getMethodDeclNode();
                    if (methodDecl->kind == naked) { continue; }
                    // TODO #3: inline method-indices in method-call-generation
                    LOCAL(methodRefOffset(superClassDecl, methodDecl), 8 * superMethodScope.index);
                    LONG(4 * methodDecl->index);
                    LONG(classTabOffset(methodDecl->scope->getClassDeclNode()));
                }
                mit.destroy();
            }
            
        }
        it.destroy();
    }
    
    // methods tab
    elem() << "\n// methods tab\n";
    LABEL(methodDeclTab());
    {
        Iterator<MethodDeclNode> &it = curClass->methods.iterator();
        while (it.hasNext()) {
            MethodDeclNode &method = it.next();
            switch (method.kind) {
                case abstract:
                case naked:
                    LONG("0");
                    break;
                default:
                    LOCAL(
                        methodDeclOffset(&method),
                        CLASS_OFFSET(methodDecl(&method))
                    );
                    LONG(methodDeclOffset(&method));
            }
        }
        it.destroy();
    }
    
    // constants
    elem() << "\n// constants";
    curClass->consts.acceptAll(*this);
    {
        Iterator<ClassScope> &it = classScope->supers();
        while (it.hasNext()) {
            ClassScope &superClassScope = it.next();
            ClassDeclNode *superClassDecl = superClassScope.getClassDeclNode();
            elem() << "\n// class-name " << superClassDecl->name << "\n";
            LOCAL(
                classNameOffset(superClassDecl),
                CLASS_OFFSET(className(superClassDecl))
            );
            LABEL(className(superClassDecl));
            ASCIZ(superClassDecl->fullQualifiedName);
        }
        it.destroy();
    }
    
    // instance template and variables
    elem() << "\n// instance template\n";
    LABEL(instanceStart());
    LONG("0"); // @class-desc
    LONG("0"); // @meminfo

    {
        Iterator<ClassScope> &it = classScope->supers();
        while (it.hasNext()) {
            ClassScope &superClassScope = it.next();
            ClassDeclNode *superClassDecl = superClassScope.getClassDeclNode();
            LABEL(instanceHandle(superClassDecl));
            LONG("0"); // _call_entry
            LONG("0"); // @inst
            LONG("0"); // vtab-offset
            
            Iterator<ClassScope> &sit = superClassScope.supers();
            while (sit.hasNext()) {
                ClassScope &ssuperClassScope = sit.next();
                ClassDeclNode *ssuperClassDecl = ssuperClassScope.getClassDeclNode();
                if (&superClassScope == classScope) {
                    LOCAL(
                        instanceHandleVarsOffset(ssuperClassDecl),
                        OFFSET(instanceHandle(superClassDecl), instanceHandleVars(ssuperClassDecl))
                    );
                    LABEL(instanceHandleVars(ssuperClassDecl));
                }
                LONG(INSTANCE_OFFSET(instanceVars(ssuperClassDecl))); // @Super-Obj-Vars
            }
            sit.destroy();
        }
        it.destroy();
    }
    
    {
        Iterator<ClassScope> &it = classScope->supers();
        while (it.hasNext()) {
            ClassScope &superClassScope = it.next();
            ClassDeclNode *superClassDecl = superClassScope.getClassDeclNode();
            LABEL(instanceVars(superClassDecl));
            superClassDecl->variables.acceptAll(*this);
        }
        it.destroy();
    }
    
    LABEL(instanceEnd());
    
    // methods
    elem() << "\n// method definitions";
    curClass->methods.acceptAll(*this);
    
    finalizeElement();
    
    return true;
}

bool X86PasmVisitor::visit(MethodDeclNode & methodDef) {
    elem() << "\n// method " << methodDef.name << "\n";
    
    switch (methodDef.kind) {
        case abstract:
            break;
        case naked:
            methodDef.body.accept(*this);
            break;
        default:
            LABEL(methodDecl(&methodDef));
            elem() << "    pushl %ebp; movl %esp, %ebp\n";
            elem() << "    \n";
            methodDef.body.accept(*this);
            elem() << "    \n";
            elem() << "    leave\n";
            elem() << "    ret\n";
    }
    return true;
}

bool X86PasmVisitor::visit(VariableDeclNode & variableDecl) {
    if (!variableDecl.scope) {
        warn() << "unresolved " << variableDecl << "\n";
        return false;
    }
    if (InstanceScope *scope = variableDecl.scope->parent->isInstance()) {
        ClassDeclNode *superClassDecl = scope->getClassDeclNode();
        elem() << "// variable " << variableDecl.name << "\n";
        LOCAL(
            instanceVarOffset(superClassDecl, &variableDecl),
            OFFSET(instanceVars(superClassDecl), instanceVar(superClassDecl, &variableDecl))
        );
        LABEL(instanceVar(superClassDecl, &variableDecl));
        LONG("0"); // TODO: size_of variable
        return true;
    }
    return false;
}

bool X86PasmVisitor::visit(VariableInitInstNode & variableInit) {
    if (!variableInit.scope) {
        warn() << "unresolved " << variableInit << "\n";
        return false;
    }
    if (ClassScope *scope = variableInit.scope->isClass()) {
        if (ConstIntExprNode *cInt = variableInit.initializer.isConstInt()) {
            VariableDeclNode &variableDecl = *variableInit.variables.first();
            elem() << "\n// int " << variableDecl.name << "\n";
            LOCAL(
                constInt(&variableDecl),
                cInt->value
            );
            return true;
        } else if (ConstCStringExprNode *cCString = variableInit.initializer.isConstCString()) {
            VariableDeclNode &variableDecl = *variableInit.variables.first();
            elem() << "\n// string " << variableDecl.name << "\n";
            LOCAL(
                constStringOffset(&variableDecl),
                CLASS_OFFSET(constString(&variableDecl))
            );
            LABEL(constString(&variableDecl));
            ASCIZ(cCString->value);
            return true;
        } else {
            return false;
        }
    }
    return false;
}

bool X86PasmVisitor::visit(BlockInstNode & block) {
    block.instructions.acceptAll(*this);
    return true;
}

bool X86PasmVisitor::visit(InlinePasmInstNode & pasmInstruction) {
    elem() << pasmInstruction.pasm << "\n";
    return true;
}
