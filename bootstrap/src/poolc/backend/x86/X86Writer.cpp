#include "poolc/backend/x86/X86Writer.hpp"

#include "poolc/storage/Types.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/ast/scopes/UnitScope.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"
#include "poolc/ast/scopes/MethodScope.hpp"
#include "poolc/ast/scopes/BlockScope.hpp"
#include "poolc/ast/scopes/VariableScope.hpp"

#include "poolc/pir/statement/PIRCond.hpp"

#define localClsPrefix(cls) (cls)->localPrefix
// TODO: replace with localClsPrefix after all inline pasm method calls are replaced pool method calls
#define manualClsPrefix(cls) (cls)->globalPrefix

#define classDesc() manualClsPrefix(curClass)
#define classTabs() localClsPrefix(curClass) << "_cts"
#define classTab(cls) localClsPrefix(curClass) << "_ct" << localClsPrefix(cls)
#define classTabDesc(cls) manualClsPrefix(cls)
#define classTabOffset(cls) localClsPrefix(curClass) << "_cto" << localClsPrefix(cls)
#define constInt(c) manualClsPrefix(curClass) << "_coi_" << (c)->name
#define constString(id) localClsPrefix(curClass) << "_cos_" << id
#define constStringOffset(id) manualClsPrefix(curClass) << "_coso_" << id
#define methodDeclTab() localClsPrefix(curClass) << "_mdt"
#define methodDecl(m) localClsPrefix((m)->scope->getClassDeclNode()) << "_md_" << (m)->name
#define methodDeclOffset(m) manualClsPrefix((m)->scope->getClassDeclNode()) << "_mdo_" << (m)->name
#define methodDeclBlock(m, b) localClsPrefix(curClass) << "_md_" << (m)->name << "_bb_" << (b)->idx
#define methodTabs() localClsPrefix(curClass) << "_mts"
#define methodTab(cls) localClsPrefix(curClass) << "_mt" << localClsPrefix(cls)

#define instanceStart() localClsPrefix(curClass) << "_tpl"
#define instanceEnd() localClsPrefix(curClass) << "_tpl_end"
#define instanceHandle(cls) localClsPrefix(curClass) << "_tpl_h" << localClsPrefix(cls)
#define instanceHandleVars(cls) localClsPrefix(curClass) << "_tpl_hv" << localClsPrefix(cls)
#define instanceHandleVarsOffset(cls) manualClsPrefix(curClass) << "_hvo" << manualClsPrefix(cls)
#define instanceVars(cls) localClsPrefix(curClass) << "_tpl_vs" << localClsPrefix(cls)
#define instanceVar(cls, var) localClsPrefix(curClass) << "_tpl_v" << localClsPrefix(cls) << "_" << (var)->name
#define instanceVarOffset(cls, var) localClsPrefix(curClass) << "_i_" << localClsPrefix(cls) << "_" << (var)->name

#define OFFSET(start, end) "(" << end << " - " << start << ")"
#define CLASS_OFFSET(end) "(" << end << " - " << classDesc() << ")"
#define INSTANCE_OFFSET(end) "(" << end << " - " << instanceStart() << ")"
#define LABEL(l) elem() << l << ":\n";
#define LONG(l) elem() << "    .long " << l << "\n";
#define ASCIZ(str) {(str).escapeToStream(elem() << "    .asciz "); elem() << "\n";}
#define LOCAL(l,v) elem() << l << " := " << v << "\n";

// public
X86Writer::X86Writer(Environment &env, MemoryInfo &mi, PoolStorage &ps, bool resolveClasses)
        :Writer(env, mi, ps, MIMETYPE_PASM), Object(env, mi), LoggerAware(env, mi),
         resolveClasses(resolveClasses),
         curClass(0), curSuper(0), curMethod(0) {}
X86Writer::~X86Writer() {}

bool X86Writer::visit(TranslationUnitNode & translationUnit) {
    translationUnit.classes.acceptAll(*this);
    return true;
}

bool X86Writer::visit(ClassDeclNode & classDef) {
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
            error() << curClass->fullQualifiedName << ": bootstrap method has to be accessible via pool-ABI.\n";
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
    LONG(constStringOffset(classScope->stringId(curClass->fullQualifiedName)));
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
            if (resolveClasses) {
                LONG(classTabDesc(superDecl));
            } else {
                LONG("0"); // @class-desc filled on class loading
            }
            LONG(constStringOffset(classScope->stringId(superDecl->fullQualifiedName)));
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
                    if (method.scope->parent->isClass()) {
                        // generate offset constant for global methods; used in bootstrapOffset header
                        LOCAL(
                            methodDeclOffset(&method),
                            CLASS_OFFSET(methodDecl(&method))
                        );
                    }
                    LONG(CLASS_OFFSET(methodDecl(&method)));
            }
        }
        it.destroy();
    }
    
    // constants
    elem() << "\n// constants";
    curClass->consts.acceptAll(*this);
    {
        Iterator<String> &it = classScope->strings();
        while (it.hasNext()) {
            String &string = it.next();
            String &id = classScope->stringId(string);
            elem() << "\n// string " << id << "\n";
            LOCAL(
                constStringOffset(id),
                CLASS_OFFSET(constString(id))
            );
            LABEL(constString(id));
            ASCIZ(string);
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

bool X86Writer::visit(MethodDeclNode & methodDef) {
    MethodScope *scope = methodDef.scope->isMethod();
    curMethod = scope->pir;
    elem() << "\n// method " << methodDef.name << "\n";
    
    switch (methodDef.kind) {
        case abstract:
            break;
        case normal:
            LABEL(methodDecl(&methodDef));
        default:
            {
                Iterator<PIRBasicBlock> &it = curMethod->blocks();
                while (it.hasNext()) {
                    write(it.next());
                }
                it.destroy();
            }
    }
    curMethod = 0;
    return true;
}

bool X86Writer::visit(VariableDeclNode & variableDecl) {
    if (!variableDecl.scope) {
        warn() << "unresolved " << variableDecl << "\n";
        return false;
    }
    if (InstanceScope *scope = variableDecl.scope->parent->isInstance()) {
        ClassDeclNode *superClassDecl = scope->getClassDeclNode();
        elem() << "// variable " << variableDecl.name << "\n";
//        if ((superClassDecl == curClass) || !resolveClasses) {
            LOCAL(
                instanceVarOffset(superClassDecl, &variableDecl),
                OFFSET(instanceVars(superClassDecl), instanceVar(superClassDecl, &variableDecl))
            );
//        }
        LABEL(instanceVar(superClassDecl, &variableDecl));
        LONG("0"); // TODO: size_of variable
        return true;
    }
    return false;
}

bool X86Writer::visit(VariableInitInstNode & variableInit) {
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
        } else {
            return false;
        }
    }
    return false;
}

void X86Writer::write(PIRBasicBlock &block) {
    MethodDeclNode *methodDecl = block.method.scope().getMethodDeclNode();
    switch (block.kind) {
        case bb_entry: {
            if (methodDecl->kind == normal) {
                code() << "pushl %ebp; movl %esp, %ebp\n";
                int localVariables = curMethod->tempCount() + curMethod->spillCount();
                if (localVariables) {
                    code() << "subl " << (4*localVariables) << ", %esp\n";
                }
                // treat all registers callee save until register allocation (#11) and enhanced inline asm (#14)
                code() << "pushad\n";
                {
                    Iterator<PIRStatement> &it = block.statements();
                    while (it.hasNext()) {
                        write(it.next());
                    }
                    it.destroy();
                }
                if (block.cond) {
                    write(*block.cond, *block.condNext);
                }
                code() << "jmp " << methodDeclBlock(methodDecl, block.next) << "\n";
            }
            break;
        }
        case bb_exit: {
            if (methodDecl->kind == normal) {
                LABEL(methodDeclBlock(methodDecl, &block));
                // treat all registers callee save until register allocation (#11) and enhanced inline asm (#14)
                code() << "popad\n";
                code() << "leave\n";
                code() << "ret\n";
            }
            break;
        }
        default: {
            LABEL(methodDeclBlock(methodDecl, &block));
            {
                Iterator<PIRStatement> &it = block.statements();
                while (it.hasNext()) {
                    write(it.next());
                }
                it.destroy();
            }
            if (block.cond) {
                write(*block.cond, *block.condNext);
            }
            if (methodDecl->kind == normal || block.next->kind == bb_block) {
                code() << "jmp " << methodDeclBlock(methodDecl, block.next) << "\n";
            }
        }
    }
}

void X86Writer::write(PIRCond &cond, PIRBasicBlock &trueBlock) {
    code() << "movl "; write(cond.left); elem() << ", %eax\n";
    code() << "cmpl "; write(cond.right); elem() << ", %eax\n";
    switch (cond.op) {
        case op_eq: code() << "je "; break;
        case op_neq: code() << "jne "; break;
        case op_lt: code() << "jlt "; break;
        case op_le: code() << "jle "; break;
        case op_gt: code() << "jgt "; break;
        case op_ge: code() << "jge "; break;
    
    } 
    elem() << methodDeclBlock(trueBlock.method.scope().getMethodDeclNode(), &trueBlock) << "\n";
}

void X86Writer::write(PIRStatement &stmt) {
    if (PIRArithOp *arithOpStmt = stmt.isArithOp()) { write(*arithOpStmt); }
    else if (PIRAsm *asmStmt = stmt.isAsm()) { write(*asmStmt); }
    else if (PIRAssign *assignStmt = stmt.isAssign()) { write(*assignStmt); }
    else if (PIRCall *callStmt = stmt.isCall()) { write(*callStmt); }
    else if (PIRGet *getStmt = stmt.isGet()) { write(*getStmt); }
    else if (PIRMove *moveStmt = stmt.isMove()) { write(*moveStmt); }
    else if (PIRSet *setStmt = stmt.isSet()) { write(*setStmt); }
    else {
        error() << "unexpected PIR statement " << stmt << "\n";
    }
}

void X86Writer::write(PIRArithOp &arithOpStmt) {
    switch (arithOpStmt.op) {
        case op_add:
            code() << "movl "; write(arithOpStmt.left); elem() << ", %eax\n";
            code() << "addl "; write(arithOpStmt.right); elem() << ", %eax\n";
            code() << "movl %eax, "; write(arithOpStmt.dest); elem() << "\n";
            break;
        case op_sub:
            code() << "movl "; write(arithOpStmt.left); elem() << ", %eax\n";
            code() << "subl "; write(arithOpStmt.right); elem() << ", %eax\n";
            code() << "movl %eax, "; write(arithOpStmt.dest); elem() << "\n";
            break;
        case op_mul:
            code() << "movl "; write(arithOpStmt.left); elem() << ", %eax\n";
            code() << "movl "; write(arithOpStmt.right); elem() << ", %ebx\n";
            code() << ".byte 0x0f; .byte 0xaf; .byte 0xc3 #//imul %ebx, %eax\n";
            code() << "movl %eax, "; write(arithOpStmt.dest); elem() << "\n";
            break;
        case op_div:
            code() << "movl "; write(arithOpStmt.left); elem() << ", %eax\n";
            code() << ".byte 0x99 #//cdq\n";
            code() << "movl "; write(arithOpStmt.right); elem() << ", %ebx\n";
            code() << ".byte 0xf7; .byte 0xfb #//idiv %ebx\n";
            code() << "movl %eax, "; write(arithOpStmt.dest); elem() << "\n";
            break;
        case op_mod:
            code() << "movl "; write(arithOpStmt.left); elem() << ", %eax\n";
            code() << ".byte 0x99 #//cdq\n";
            code() << "movl "; write(arithOpStmt.right); elem() << ", %ebx\n";
            code() << ".byte 0xf7; .byte 0xfb #//idiv %ebx\n";
            code() << "movl %edx, "; write(arithOpStmt.dest); elem() << "\n";
            break;
    }
}

void X86Writer::write(PIRAsm &asmStmt) {
    {
        Iterator<String> &it = asmStmt.in.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            code() << "movl "; write(asmStmt.in.get(reg)); elem() << ", " << reg << "\n";
        }
        it.destroy();
    }
    elem() << asmStmt.pasm << "\n";
    {
        Iterator<String> &it = asmStmt.out.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            code() << "movl " << reg << ", "; write(asmStmt.out.get(reg)); elem() << "\n";
        }
        it.destroy();
    }
}

void X86Writer::write(PIRAssign &assignStmt) {
    if (PIRInt *intValue = assignStmt.val.isInt()) {
        code() << "movl " << intValue->value << ", ";
    } else if (PIRString *stringValue = assignStmt.val.isString()) {
        code() << "movl 8(%ebp), %eax\n";
        code() << "addl " << constStringOffset(stringValue->id) << ", %eax\n";
        code() << "movl %eax, ";
    } else { // isNull
        code() << "movl 0, ";
    }
    write(assignStmt.dest);
    elem() << "\n";
}

void X86Writer::write(PIRCall &callStmt) {
    int retCount = callStmt.rets.size();
    int paramCount = callStmt.params.size();
    if (retCount) {
        code() << "subl " << (4*retCount) << ", %esp\n";
    }
    {
        Iterator<PIRLocation> &it = callStmt.params.iterator();
        pushAllReverse(it);
        it.destroy();
    }
    code() << "movl "; write(callStmt.context); elem() << ", %eax\n";
    code() << "pushl %eax; pushl " << (8*callStmt.method.index) << "; call (%eax)\n";
    code() << "addl " << (8 + 4*paramCount) << ", %esp\n";
    if (retCount) {
        Iterator<PIRLocation> &it = callStmt.rets.iterator();
        while (it.hasNext()) {
            code() << "popl "; write(it.next()); elem() << "\n";
        }
        it.destroy();
    }
}
            
void X86Writer::write(PIRGet &getStmt) {
    VariableDeclNode *decl = getStmt.variable.getVariableDeclNode();
    ClassDeclNode *declClass = getStmt.variable.getClassDeclNode();
     
    code() << "movl "; write(getStmt.context); elem() << ", %eax\n";
    code() << "movl " << instanceHandleVarsOffset(declClass) << "(%eax), %ebx\n";
    code() << "addl 4(%eax), %ebx\n";
    code() << "movl " << instanceVarOffset(declClass, decl) << "(%ebx), %eax\n";
    code() << "movl %eax, "; write(getStmt.dest); elem() << "\n";
}

void X86Writer::write(PIRMove &moveStmt) {
    code() << "movl "; write(moveStmt.src); elem() << ", %eax\n";
    code() << "movl %eax, "; write(moveStmt.dest); elem() << "\n";
}

void X86Writer::write(PIRSet &setStmt) {
    VariableDeclNode *decl = setStmt.variable.getVariableDeclNode();
    ClassDeclNode *declClass = setStmt.variable.getClassDeclNode();
     
    code() << "movl "; write(setStmt.context); elem() << ", %eax\n";
    code() << "movl " << instanceHandleVarsOffset(declClass) << "(%eax), %ebx\n";
    code() << "addl 4(%eax), %ebx\n";
    code() << "movl "; write(setStmt.src); elem() << ", %eax\n";
    code() << "movl %eax, " << instanceVarOffset(declClass, decl) << "(%ebx)\n";
}

void X86Writer::write(PIRLocation &location) {
    int offset = 0;
    switch (location.kind) {
        case loc_param:
            offset = 16 + 4*location.idx;
            break;
        case loc_ret:
            offset = 16 + 4*curMethod->paramCount() + 4*location.idx;
            break;
        case loc_spill:
            offset = -4 - 4*location.idx;
            break;
        case loc_temp:
            offset = -4 - 4*curMethod->spillCount() - 4*location.idx;
            break;
        case loc_this:
            offset = 12;
            break;
    }
    elem() << offset << "(%ebp)";
}

// private
OStream & X86Writer::code() {
    return elem() << "    ";
}
    
void X86Writer::pushAllReverse(Iterator<PIRLocation> &it) {
    if (!it.hasNext()) {
        return;
    }
    PIRLocation &loc = it.next();
    pushAllReverse(it);
    code() << "pushl "; write(loc); elem() << "\n";
}
