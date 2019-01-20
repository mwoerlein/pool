#include "poolc/pir/PIRBasicBlock.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"

#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"

#include "poolc/pir/statement/PIRArithOp.hpp"
#include "poolc/pir/statement/PIRAsm.hpp"
#include "poolc/pir/statement/PIRAssign.hpp"
#include "poolc/pir/statement/PIRCall.hpp"
#include "poolc/pir/statement/PIRCond.hpp"
#include "poolc/pir/statement/PIRGet.hpp"
#include "poolc/pir/statement/PIRMove.hpp"
#include "poolc/pir/statement/PIRSet.hpp"

// public
PIRBasicBlock::PIRBasicBlock(Environment &env, MemoryInfo &mi, bb_kind kind, PIRMethod &method, int idx)
        :Object(env, mi), LoggerAware(env, mi), kind(kind), method(method), idx(idx),
         cond(0), next(0), condNext(0),
         _statements(env.create<LinkedList<PIRStatement>>()) {}
PIRBasicBlock::~PIRBasicBlock() {
    {
        Iterator<PIRStatement> &it = _statements.iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        _statements.destroy();
    }
}

OStream & PIRBasicBlock::operator >>(OStream & stream) {
    switch (kind) {
        case bb_entry:
            return stream << "entry-block";
        case bb_exit:
            return stream << "exit-block";
    }
    return stream << "basic-block-" << idx;
}

void PIRBasicBlock::setCondNext(logical_op op, PIRLocation &left, PIRLocation &right, PIRBasicBlock &trueNext, PIRBasicBlock &falseNext) {
    if ((op == op_and) || (op == op_or)) {
        error() << "invalid operator for condition: " << left << " <op> " << right << "\n";
        return;
    }
    if (!left.type.isInt() || !right.type.isInt()) {
        error() << "invalid types for condition: " << left << " <op> " << right << "\n";
        return;
    }
    this->cond = &env().create<PIRCond, logical_op, PIRLocation&, PIRLocation&>(op, left, right);
    this->condNext = &trueNext;
    this->next = &falseNext;
}

void PIRBasicBlock::addArithOp(arith_op op, PIRLocation &left, PIRLocation &right, PIRLocation &dest) {
    if (!left.type.isInt() || !right.type.isInt() || !dest.type.isInt()) {
        error() << "invalid types for arithmetic operation: " << dest << " = " << left << " <op> " << right << "\n";
        return;
    }
    _statements.add(env().create<PIRArithOp, arith_op, PIRLocation&, PIRLocation&, PIRLocation&>(op, left, right, dest));
}

void PIRBasicBlock::addAsm(String & pasm, Map<String, PIRLocation> &in, Map<String, PIRLocation> &out) {
    _statements.add(env().create<PIRAsm, String&, Map<String, PIRLocation> &, Map<String, PIRLocation> &>(pasm, in, out));
}

void PIRBasicBlock::addAssign(PIRValue &value, PIRLocation &dest) {
    if (value.isNull()) {
        if (!dest.type.isInstance() && !dest.type.isAll() && !dest.type.isAny()) {
            error() << "incompatible destination " << dest << " for null\n";
            return;
        }
    } else if (value.isInt()) {
        if (!dest.type.isInt()) {
            error() << "incompatible destination " << dest << " for int constant\n";
            return;
        }
    } else if (value.isString()) {
        if (!dest.type.isCString()) {
            error() << "incompatible destination " << dest << " for string constant\n";
            return;
        }
    }
    _statements.add(env().create<PIRAssign, PIRValue&, PIRLocation&>(value, dest));
}

void PIRBasicBlock::addCall(PIRLocation &context, MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets) {
    MethodDeclNode &decl = *method.getMethodDeclNode();
    if (method.getInstance() != &context.type) {
        error() << "invalid context " << context << " of method " << decl << "\n";
        return;
    }
    {
        if (decl.parameters.size() != params.size()) {
            error() << "invalid parameter count " << params.size() << " for method " << decl << "\n";
            return;
        }
        Iterator<VariableDeclNode> &declIt = decl.parameters.iterator();
        Iterator<PIRLocation> &locIt = params.iterator();
        while (declIt.hasNext()) {
            VariableDeclNode &vDecl = declIt.next();
            PIRLocation &loc = locIt.next();
            // TODO: handle type conversion?
            if (!isAssignable(loc.type, *vDecl.resolvedType)) {
                error() << "incompatible parameter " << loc << " for method " << decl << "\n";
                error() << *vDecl.resolvedType << " <-> " << loc.type << "\n";
                return;
            }
        }
        declIt.destroy();
        locIt.destroy();
    }
    {
        if (decl.returnTypes.size() != rets.size()) {
            error() << "invalid return count " << rets.size() << " for method " << decl << "\n";
            return;
        }
        Iterator<Type> &typeIt = decl.resolvedReturns.iterator();
        Iterator<PIRLocation> &locIt = rets.iterator();
        while (typeIt.hasNext()) {
            Type &type = typeIt.next();
            PIRLocation &loc = locIt.next();
            // TODO: handle type conversion?
            if (!isAssignable(type, loc.type)) {
                error() << "incompatible return " << loc << " for method " << decl << "\n";
                return;
            }
        }
        typeIt.destroy();
        locIt.destroy();
    }
    _statements.add(env().create<PIRCall, PIRLocation &, MethodScope &, Collection<PIRLocation> &, Collection<PIRLocation> &>(context, method, params, rets));
}

void PIRBasicBlock::addGet(PIRLocation &context, VariableScope &var, PIRLocation &dest) {
    VariableDeclNode &decl = *var.getVariableDeclNode();
    if (InstanceScope *contextScope = context.type.isInstance()) {
        if (!contextScope->getClass()->hasSuper(*var.getClass())) {
            error() << "incompatible context " << context << " for variable " << decl << "\n";
            return;
        }
    } else {
        error() << "invalid context " << context << " for variable " << decl << "\n";
        return;
    }
    // TODO: handle type conversion?
    if (!isAssignable(*decl.resolvedType, dest.type)) {
        error() << "incompatible destination " << dest << " for variable " << decl << "\n";
        return;
    }
    _statements.add(env().create<PIRGet, PIRLocation &, VariableScope &, PIRLocation &>(context, var, dest));
}

void PIRBasicBlock::addMove(PIRLocation &src, PIRLocation &dest, bool reinterpret) {
    if (!reinterpret) {
        // TODO: handle type conversion?
        if (!isAssignable(src.type, dest.type)) {
            error() << "incompatible types for move: " << src << " <-> " << dest << "\n";
            return;
        }
    }
    _statements.add(env().create<PIRMove, PIRLocation&, PIRLocation&>(src, dest));
}

void PIRBasicBlock::addSet(PIRLocation &context, VariableScope &var, PIRLocation &src) {
    VariableDeclNode &decl = *var.getVariableDeclNode();
    if (InstanceScope *contextScope = context.type.isInstance()) {
        if (!contextScope->getClass()->hasSuper(*var.getClass())) {
            error() << "incompatible context " << context << " for variable " << decl << "\n";
            return;
        }
    } else {
        error() << "invalid context " << context << " for variable " << decl << "\n";
        return;
    }
    // TODO: handle type conversion?
    if (!isAssignable(src.type, *decl.resolvedType)) {
        error() << "incompatible source " << src << " for variable " << decl << " " << decl.resolvedType << "\n";
        return;
    }
    _statements.add(env().create<PIRSet, PIRLocation &, VariableScope &, PIRLocation &>(context, var, src));
}

// private    
bool PIRBasicBlock::isAssignable(Type &src, Type &dest) {
    if (&src == &dest) { return true; }
    
    AllType *srcAll = src.isAll();
    AnyType *srcAny = src.isAny();
    InstanceScope *srcInstance = src.isInstance();
    if (!srcAll && !srcInstance && !srcAny) { return false; }
    
    AllType *destAll = dest.isAll();
    AnyType *destAny = dest.isAny();
    InstanceScope *destInstance = dest.isInstance();
    if (!destAll && !destInstance && !destAny) { return false; }
    
    // TODO: handle type conversion?
    return srcAll || destAny;
}
