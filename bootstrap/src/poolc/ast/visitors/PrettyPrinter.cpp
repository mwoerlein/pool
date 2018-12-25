#include "poolc/ast/visitors/PrettyPrinter.hpp"

#include "poolc/storage/Types.hpp"

#include "poolc/ast/nodes/all.hpp"

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
    translationUnit.classes.acceptAll(*this);
    
    finalizeElement();
    return true;
}

bool PrettyPrinter::visit(NamespaceDeclNode & namespaceDef) {
    indent() << "namespace " << namespaceDef.name << ";\n";
}

bool PrettyPrinter::visit(UseStatementNode & useStmt) {
    OStream & line = indent() << "use " << useStmt.name;
    if (useStmt.alias != useStmt.name.lastPart()) {
        line << " as " << useStmt.alias;
    }
    line << ";\n";
}

bool PrettyPrinter::visit(ClassDeclNode & classDef) {
    OStream & line = indent() << "class " << classDef.name;
    if (classDef.extends.size()) {
        line << " extends";
        Iterator<TypeRefNode> &it = classDef.extends.iterator();
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
    
    if (classDef.consts.size()) {
        indent() << "\n";
        indent() << "// constants\n";
        classDef.consts.acceptAll(*this);
    }
    
    if (classDef.variables.size()) {
        indent() << "\n";
        indent() << "// variables\n";
        classDef.variables.acceptAll(*this);
    }
    
    if (classDef.methods.size()) {
        indent() << "\n";
        indent() << "// methods\n";
        classDef.methods.acceptAll(*this);
    }
    
    endBlock();
}

bool PrettyPrinter::visit(MethodDeclNode & methodDef) {
    OStream & line = indent();
    switch (methodDef.kind) {
        case abstract:
            line << "abstract ";
            break;
        case naked:
            line << "__naked__ ";
            break;
        default:
            if (methodDef.scope == scope_class) {
                line << "global ";
            }
    }
    line << "<";
    // TODO: #3 handle return types
    line << "> " << methodDef.name << "(";
    // TODO: #3 handle parameters
    line << ")";
    switch (methodDef.kind) {
        case abstract:
            line << ";\n";
            break;
        default:
            startBlock(&line);
            endBlock();
    }
    indent() << "\n";
}

bool PrettyPrinter::visit(VariableInitInstNode & variableInit) {
    bool tuple = variableInit.variables.size() > 1;
    OStream & line = indent();
    
    if (variableInit.global) { line << "global "; }
    if (tuple) { line << "<"; }
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
    if (tuple) { line << ">"; }
    line << (variableInit.final ? " := " : " = ");
    variableInit.initializer.accept(*this);
    line << ";\n";
}

bool PrettyPrinter::visit(VariableDeclNode & variableDef) {
    OStream & line = indent();
    variableDef.type.accept(*this);
    line << " " << variableDef.name << ";\n";
}

bool PrettyPrinter::visit(ClassRefNode & classRef) { elem() << classRef.name; }
bool PrettyPrinter::visit(CStringRefNode & type) { elem() << "cstring"; }
bool PrettyPrinter::visit(IntRefNode & type) { elem() << "int"; }

bool PrettyPrinter::visit(ConstCStringExprNode & constCString) { constCString.value.escapeToStream(elem()); }
bool PrettyPrinter::visit(ConstIntExprNode & constInt) { elem() << constInt.value; }

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
