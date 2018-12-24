#include "poolc/ast/Node.hpp"

// public
Node::~Node() {}

OStream & Node::operator >>(OStream & stream) {
    printDebugName(stream);
    return stream;
}

void Node::printDebugInfo(OStream & stream) {
    stream << "(" << first_line << "/" << first_column << " - " << last_line << "/" << last_column << ")";
}

