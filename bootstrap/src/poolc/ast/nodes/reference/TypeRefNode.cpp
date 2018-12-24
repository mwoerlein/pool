#include "poolc/ast/nodes/reference/TypeRefNode.hpp"

// public
TypeRefNode::~TypeRefNode() {}

ClassRefNode * TypeRefNode::isClass() { return 0; }
IntRefNode * TypeRefNode::isInt() { return 0; }
CStringRefNode * TypeRefNode::isCString() { return 0; }
