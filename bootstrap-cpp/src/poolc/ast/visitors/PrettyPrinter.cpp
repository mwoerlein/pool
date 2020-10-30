#include "poolc/ast/visitors/PrettyPrinter.hpp"

#include "poolc/storage/Types.hpp"

#include "poolc/ast/nodes/all.hpp"

#include "poolc/pir/PIRLocation.hpp"
#include "poolc/pir/PIRMethod.hpp"
#include "poolc/pir/PIRStatement.hpp"
#include "poolc/pir/statement/PIRCond.hpp"

// public
PrettyPrinter::PrettyPrinter(Environment &env, MemoryInfo &mi, PoolStorage &ps)
        :Writer(env, mi, ps, MIMETYPE_POOL), Object(env, mi), LoggerAware(env, mi),
         _indent(0) {
}
PrettyPrinter::~PrettyPrinter() {
}
    
bool PrettyPrinter::visit(TranslationUnitNode & translationUnit) {
    if (!startElement(translationUnit.name)) {
        return false;
    }
    
    translationUnit.element.dumpHeaders(elem());
    
    translationUnit.ns->accept(*this);
    indent() << "\n";
    translationUnit.uses.acceptAll(*this);
    indent() << "\n";
    translationUnit.structs.acceptAll(*this);
    translationUnit.classes.acceptAll(*this);
    
    finalizeElement();
    return true;
}

bool PrettyPrinter::visit(NamespaceDeclNode & namespaceDecl) {
    indent() << "namespace " << namespaceDecl.name << ";\n";
    return true;
}

bool PrettyPrinter::visit(UseStatementNode & useStmt) {
    OStream & line = indent() << "use " << useStmt.name;
    if (useStmt.alias != useStmt.name.lastPart()) {
        line << " as " << useStmt.alias;
    }
    line << ";\n";
    return true;
}

bool PrettyPrinter::visit(ClassDeclNode & classDecl) {
    OStream & line = indent() << "class " << classDecl.name;
    if (classDecl.extends.size()) {
        line << " extends";
        Iterator<TypeRefNode> &it = classDecl.extends.iterator();
        if (it.hasNext()) {
            if (ClassRefNode *extend = it.next().isClass()) {
                line << " " << extend->name;
            }
        }
        while (it.hasNext()) {
            if (ClassRefNode *extend = it.next().isClass()) {
                line << ", " << extend->name;
            }
        }
        it.destroy();
    }
    startBlock(&line);
    
    if (classDecl.consts.size()) {
        indent() << "\n";
        indent() << "// constants\n";
        classDecl.consts.acceptAll(*this);
    }
    
    if (classDecl.variables.size()) {
        indent() << "\n";
        indent() << "// variables\n";
        classDecl.variables.acceptAll(*this);
    }
    
    if (classDecl.methods.size()) {
        indent() << "\n";
        indent() << "// methods\n";
        classDecl.methods.acceptAll(*this);
    }
    
    endBlock();
    return true;
}

bool PrettyPrinter::visit(StructDeclNode & structDecl) {
    OStream & line = indent() << "struct " << structDecl.name;
    startBlock(&line);
    
    if (structDecl.consts.size()) {
        indent() << "\n";
        indent() << "// constants\n";
        structDecl.consts.acceptAll(*this);
    }
    
    if (structDecl.variables.size()) {
        indent() << "\n";
        indent() << "// variables\n";
        structDecl.variables.acceptAll(*this);
    }
    
    endBlock();
    return true;
}

bool PrettyPrinter::visit(MethodDeclNode & methodDecl) {
    OStream & line = indent();
    switch (methodDecl.kind) {
        case abstract:
            line << "abstract ";
            break;
        case entry:
            line << "__entry__ ";
            break;
        case naked:
            line << "__naked__ ";
            break;
        default:
            if (methodDecl.global) {
                line << "global ";
            }
    }
    line << "[";
    {
        Iterator<TypeRefNode> &it = methodDecl.returnTypes.iterator();
        while (it.hasNext()) {
            it.next().accept(*this);
            if (it.hasNext()) { line << ", "; }
        }
        it.destroy();
    }
    line << "] " << methodDecl.name << "(";
    {
        Iterator<VariableDeclNode> &it = methodDecl.parameters.iterator();
        while (it.hasNext()) {
            VariableDeclNode & var = it.next();
            var.type.accept(*this);
            line << " " << var.name;
            if (it.hasNext()) { line << ", "; }
        }
        it.destroy();
    }
    line << ")";
    switch (methodDecl.kind) {
        case abstract:
            line << ";\n";
            break;
        default:
            if (methodDecl.body.isEmpty()) {
                line << " {}\n";
            } else {
                startBlock(&line);
                methodDecl.body.instructions.acceptAll(*this);
                endBlock();
            }
    }
    indent() << "\n";
    return true;
}

bool PrettyPrinter::visit(VariableInitInstNode & variableInit) {
    bool tuple = variableInit.variables.size() > 1;
    OStream & line = indent();
    
    if (variableInit.global) { line << "global "; }
    if (tuple) { line << "["; }
    {
        Iterator<VariableDeclNode> &it = variableInit.variables.iterator();
        while (it.hasNext()) {
            VariableDeclNode & var = it.next();
            var.type.accept(*this);
            line << " " << var.name;
            if (it.hasNext()) { line << ", "; }
        }
        it.destroy();
    }
    if (tuple) { line << "]"; }
    line << (variableInit.final ? (variableInit.reinterpret ? " ::= " : " := ") : " = ");
    variableInit.initializer.accept(*this);
    line << ";\n";
    return true;
}

bool PrettyPrinter::visit(VariableDeclNode & variableDecl) {
    OStream & line = indent();
    variableDecl.type.accept(*this);
    line << " " << variableDecl.name << ";\n";
    return true;
}

bool PrettyPrinter::visit(BlockInstNode & block) {
    if (block.isEmpty()) {
        indent() << "{}\n";
    } else {
        startBlock();
        block.instructions.acceptAll(*this);
        endBlock();
    }
    return true;
}

bool PrettyPrinter::visit(ExpressionInstNode & exprInst) {
    OStream & line = indent();
    exprInst.expression.accept(*this);
    line << ";\n";
    return true;
}

bool PrettyPrinter::visit(IfInstNode & ifInst) {
    OStream & line = indent() << "if (";
    ifInst.condition.accept(*this);
    line << ")";
    
    if (ifInst.trueBlock.isEmpty()) {
        line << " {}";
        if (ifInst.falseBlock.isEmpty()) {
            line << "\n";
        } else {
            line << " else";
            startBlock(&line);
            ifInst.falseBlock.instructions.acceptAll(*this);
            endBlock();
        }
    } else {
        startBlock(&line);
        ifInst.trueBlock.instructions.acceptAll(*this);
        if (!ifInst.falseBlock.isEmpty()) {
            _indent -= 4;
            indent() << "} else {\n";
            _indent += 4;
            ifInst.falseBlock.instructions.acceptAll(*this);
        }
        endBlock();
    }
    return true;
}

bool PrettyPrinter::visit(InlinePasmInstNode & pasmInst) {
    indent() << "__pasm__(<\"\n";
    elem() << pasmInst.pasm << "\n";
    indent() << "\">, {";
    {
        Iterator<String> &it = pasmInst.in.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            reg.escapeToStream(elem());
            elem() << ": ";
            pasmInst.in.get(reg).accept(*this);
            if (it.hasNext()) { elem() << ", "; }
        }
        it.destroy();
    }
    elem() << "}, {";
    {
        Iterator<String> &it = pasmInst.out.keys();
        while (it.hasNext()) {
            String &reg = it.next();
            reg.escapeToStream(elem());
            elem() << ": ";
            pasmInst.out.get(reg).accept(*this);
            if (it.hasNext()) { elem() << ", "; }
        }
        it.destroy();
    }
    elem() << "});\n";
    return true;
}

bool PrettyPrinter::visit(ReturnInstNode & returnInst) {
    if (returnInst.values.isEmpty()) {
        indent() << "return;\n";
        return true;
    }
    bool tuple = returnInst.values.size() > 1;
    
    OStream & line = indent() << "return ";
    if (tuple) { line << "["; }
    {
        Iterator<ExpressionNode> &it = returnInst.values.iterator();
        while (it.hasNext()) {
            it.next().accept(*this);
            if (it.hasNext()) { line << ", "; }
        }
        it.destroy();
    }
    if (tuple) { line << "]"; }
    line << ";\n";
    return true;
}

bool PrettyPrinter::visit(WhileInstNode & whileInst) {
    OStream & line = indent() << "while (";
    whileInst.condition.accept(*this);
    line << ")";
    
    if (whileInst.block.isEmpty()) {
        line << " {}\n";
    } else {
        startBlock(&line);
        whileInst.block.instructions.acceptAll(*this);
        endBlock();
    }
    return true;
}

bool PrettyPrinter::visit(ArithAssignmentExprNode & arithAssignment) {
    elem() << "(";
    arithAssignment.variable.accept(*this);
    switch (arithAssignment.op) {
        case op_add: elem() << " += "; break;
        case op_sub: elem() << " -= "; break;
        case op_mul: elem() << " *= "; break;
        case op_div: elem() << " /= "; break;
        case op_mod: elem() << " %= "; break;
    }
    arithAssignment.value.accept(*this);
    elem() << ")";
    return true;
}
bool PrettyPrinter::visit(ArithBinaryExprNode & arithBinary) {
    elem() << "(";
    arithBinary.left.accept(*this);
    switch (arithBinary.op) {
        case op_add: elem() << " + "; break;
        case op_sub: elem() << " - "; break;
        case op_mul: elem() << " * "; break;
        case op_div: elem() << " / "; break;
        case op_mod: elem() << " % "; break;
    }
    arithBinary.right.accept(*this);
    elem() << ")";
    return true;
}
bool PrettyPrinter::visit(ArithUnaryExprNode & arithUnary) {
    elem() << "(";
    switch (arithUnary.op) {
        case unary_inc: elem() << "++"; arithUnary.variable.accept(*this); break;
        case unary_dec: elem() << "--"; arithUnary.variable.accept(*this); break;
        case unary_post_inc: arithUnary.variable.accept(*this); elem() << "++"; break;
        case unary_post_dec: arithUnary.variable.accept(*this); elem() << "--"; break;
    }
    elem() << ")";
    return true;
}
bool PrettyPrinter::visit(AssignmentExprNode & assignment) {
    elem() << "(";
    assignment.variable.accept(*this);
    elem() << " = ";
    assignment.value.accept(*this);
    elem() << ")";
    return true;
}
bool PrettyPrinter::visit(ConstCStringExprNode & constCString) {
    constCString.value.escapeToStream(elem());
    return true;
}
bool PrettyPrinter::visit(ConstIntExprNode & constInt) {
    elem() << constInt.value;
    return true;
}
bool PrettyPrinter::visit(LogicalBinaryExprNode & logicalBinary) {
    elem() << "(";
    logicalBinary.left.accept(*this);
    switch (logicalBinary.op) {
        case op_and: elem() << " && "; break;
        case op_or: elem() << " || "; break;
        case op_eq: elem() << " == "; break;
        case op_neq: elem() << " != "; break;
        case op_lt: elem() << " < "; break;
        case op_le: elem() << " <= "; break;
        case op_gt: elem() << " > "; break;
        case op_ge: elem() << " >= "; break;
    }
    logicalBinary.right.accept(*this);
    elem() << ")";
    return true;
}
bool PrettyPrinter::visit(LogicalUnaryExprNode & logicalUnary) {
    elem() << "(";
    switch (logicalUnary.op) {
        case unary_not: elem() << "!"; logicalUnary.expression.accept(*this); break;
    }
    elem() << ")";
    return true;
}
bool PrettyPrinter::visit(MethodCallExprNode & methodCall) {
    methodCall.context.accept(*this);
    elem() << "." << methodCall.name << "(";
    {
        Iterator<ExpressionNode> &it = methodCall.parameters.iterator();
        while (it.hasNext()) {
            it.next().accept(*this);
            if (it.hasNext()) { elem() << ", "; }
        }
        it.destroy();
    }
    elem() << ")";
    return true;
}
bool PrettyPrinter::visit(NullExprNode & constNull) {
    elem() << "null";
    return true;
}
bool PrettyPrinter::visit(SignExprNode & sign) {
    elem() << "(";
    switch (sign.sign) {
        case sign_plus: elem() << "+"; sign.expression.accept(*this); break;
        case sign_minus: elem() << "-"; sign.expression.accept(*this); break;
    }
    elem() << ")";
    return true;
}
bool PrettyPrinter::visit(ThisExprNode & constThis) {
    elem() << "this";
    return true;
}
bool PrettyPrinter::visit(VariableExprNode & variable) {
    if (variable.context) {
        variable.context->accept(*this);
        if (variable.context->resolvedType && variable.context->resolvedType->isClass()) {
            elem() << ":";
        } else {
            elem() << ".";
        }
    }
    elem() << variable.name;
    return true;
}

bool PrettyPrinter::visit(AllRefNode & type) { elem() << "__all__"; return true; }
bool PrettyPrinter::visit(AnyRefNode & type) { elem() << "__any__"; return true; }
bool PrettyPrinter::visit(ClassRefNode & classRef) { elem() << classRef.name; return true; }
bool PrettyPrinter::visit(CStringRefNode & type) { elem() << "cstring"; return true; }
bool PrettyPrinter::visit(IntRefNode & type) { elem() << "int"; return true; }

// private
OStream & PrettyPrinter::indent() {
    OStream & line = elem();
    int i = _indent;
    while (i-- > 0) {
        line << " ";
    }
    return line;
}
void PrettyPrinter::startBlock(OStream *line) {
    if (line) {
        *line << " {\n";
    } else {
        indent() << "{\n";
    }
    _indent += 4;
}
void PrettyPrinter::endBlock() {
    _indent -= 4;
    indent() << "}\n";
}
