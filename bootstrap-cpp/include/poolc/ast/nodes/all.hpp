#include "poolc/ast/nodes/TranslationUnitNode.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/NamespaceDeclNode.hpp"
#include "poolc/ast/nodes/declaration/StructDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"

#include "poolc/ast/nodes/expression/ArithAssignmentExprNode.hpp"
#include "poolc/ast/nodes/expression/ArithBinaryExprNode.hpp"
#include "poolc/ast/nodes/expression/ArithUnaryExprNode.hpp"
#include "poolc/ast/nodes/expression/AssignmentExprNode.hpp"
#include "poolc/ast/nodes/expression/ConstCStringExprNode.hpp"
#include "poolc/ast/nodes/expression/ConstIntExprNode.hpp"
#include "poolc/ast/nodes/expression/LogicalBinaryExprNode.hpp"
#include "poolc/ast/nodes/expression/LogicalUnaryExprNode.hpp"
#include "poolc/ast/nodes/expression/MethodCallExprNode.hpp"
#include "poolc/ast/nodes/expression/NullExprNode.hpp"
#include "poolc/ast/nodes/expression/SignExprNode.hpp"
#include "poolc/ast/nodes/expression/ThisExprNode.hpp"
#include "poolc/ast/nodes/expression/VariableExprNode.hpp"

#include "poolc/ast/nodes/instruction/BlockInstNode.hpp"
#include "poolc/ast/nodes/instruction/ExpressionInstNode.hpp"
#include "poolc/ast/nodes/instruction/IfInstNode.hpp"
#include "poolc/ast/nodes/instruction/InlinePasmInstNode.hpp"
#include "poolc/ast/nodes/instruction/ReturnInstNode.hpp"
#include "poolc/ast/nodes/instruction/VariableInitInstNode.hpp"
#include "poolc/ast/nodes/instruction/WhileInstNode.hpp"

#include "poolc/ast/nodes/reference/AllRefNode.hpp"
#include "poolc/ast/nodes/reference/AnyRefNode.hpp"
#include "poolc/ast/nodes/reference/ClassRefNode.hpp"
#include "poolc/ast/nodes/reference/CStringRefNode.hpp"
#include "poolc/ast/nodes/reference/IntRefNode.hpp"
#include "poolc/ast/nodes/reference/UseStatementNode.hpp"
