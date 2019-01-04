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

VariableInitInstNode &ClassDeclNode::addStringConstant(String &name, String &value) {
    TypeRefNode &ref = env().create<CStringRefNode>();
    VariableDeclNode &decl = env().create<VariableDeclNode, TypeRefNode&, String&>(ref, name);
    ConstCStringExprNode &cstring = env().create<ConstCStringExprNode, String&>(value);
    VariableInitInstNode &init = env().create<VariableInitInstNode, VariableDeclNode &, ExpressionNode &>(decl, cstring);
    consts.add(init);
    return init;
}

bool ClassDeclNode::accept(Visitor & visitor) {
    return visitor.visit(*this);
}

void ClassDeclNode::printDebugName(OStream & stream) {
    stream << "class-decl-<" << name << ">";
}
