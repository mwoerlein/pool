#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

// public
TypeRefNode::~TypeRefNode() {}

AllRefNode * TypeRefNode::isAll() { return 0; }
AnyRefNode * TypeRefNode::isAny() { return 0; }
ClassRefNode * TypeRefNode::isClass() { return 0; }
CStringRefNode * TypeRefNode::isCString() { return 0; }
IntRefNode * TypeRefNode::isInt() { return 0; }
