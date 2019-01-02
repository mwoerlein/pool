#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"

#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"

#include "poolc/ast/nodes/instruction/VariableInitInstNode.hpp"

#include "poolc/ast/nodes/reference/ClassRefNode.hpp"

#include "sys/collection/LinkedList.hpp"
#include "sys/collection/HashMap.hpp"

#include "poolc/ast/nodes/expression/ConstCStringExprNode.hpp"
#include "poolc/ast/nodes/reference/CStringRefNode.hpp"
#include "poolc/ast/scopes/ClassScope.hpp"


// public
ClassDeclNode::ClassDeclNode(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         name(env.create<String, const char *>("UNKNOWN")),
         fullQualifiedName(env.create<FullQualifiedName>()),
         globalPrefix(env.create<String>()),
         localPrefix(env.create<String>()),
         instanceScope(0),
         extends(env.create<NodeList<TypeRefNode>>()),
         variables(env.create<NodeList<VariableDeclNode>>()),
         consts(env.create<NodeList<VariableInitInstNode>>()),
         methods(env.create<NodeList<MethodDeclNode>>()) {
}
ClassDeclNode::~ClassDeclNode() {
    name.destroy();
    fullQualifiedName.destroy();
    globalPrefix.destroy();
    localPrefix.destroy();
    
    extends.destroyAll();
    variables.destroyAll();
    consts.destroyAll();
    methods.destroyAll();
}

VariableScope *ClassDeclNode::registerConstantCString(ConstCStringExprNode &value) {
    ClassScope *classScope = this->scope->isClass();
    String &constName = env().create<String>();
    
    // TODO: handle hash collision
    constName << "_" << value.value.hash();
    VariableScope *ret = classScope->getVariable(constName);
    if (ret) {
        constName.destroy();
        return ret;
    }
    
    TypeRefNode &ref = env().create<CStringRefNode>();
    VariableDeclNode &decl = env().create<VariableDeclNode, TypeRefNode&, String&>(ref, constName);
    ConstCStringExprNode &cstring = env().create<ConstCStringExprNode, String&>(value.value);
    VariableInitInstNode &init = env().create<VariableInitInstNode, VariableDeclNode &, ExpressionNode &>(decl, cstring);
    classScope->getClassDeclNode()->consts.add(init);
    
    init.scope = cstring.scope = ref.scope = classScope;
    cstring.resolvedType = ref.resolvedType = value.resolvedType;
    
    VariableScope *variableScope = classScope->registerVariable(decl);
    decl.scope = variableScope;
    decl.resolvedType = value.resolvedType;
    return variableScope;
}

bool ClassDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ClassDeclNode::printDebugName(OStream & stream) {
    stream << "class-decl-<" << name << ">";
}
