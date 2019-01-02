#include "poolc/pir/PIRMethod.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"

#include "poolc/pir/statement/PIRAsm.hpp"
#include "poolc/pir/statement/PIRAssign.hpp"
#include "poolc/pir/statement/PIRCall.hpp"
#include "poolc/pir/statement/PIRGet.hpp"
#include "poolc/pir/statement/PIRMove.hpp"
#include "poolc/pir/statement/PIRReturn.hpp"
#include "poolc/pir/statement/PIRSet.hpp"

// public
PIRMethod::PIRMethod(Environment &env, MemoryInfo &mi)
        :Object(env, mi), LoggerAware(env, mi),
         _params(env.create<LinkedList<PIRLocation>>()),
         _rets(env.create<LinkedList<PIRLocation>>()),
         _spills(env.create<LinkedList<PIRLocation>>()),
         _temps(env.create<LinkedList<PIRLocation>>()),
         _statements(env.create<LinkedList<PIRStatement>>()),
         _null(env.create<PIRNull>()) {}
PIRMethod::~PIRMethod() {
    _null.destroy();
    if (_this) { _this->destroy(); }
    {
        Iterator<PIRLocation> &it = _params.iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        _params.destroy();
    }
    {
        Iterator<PIRLocation> &it = _rets.iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        _rets.destroy();
    }
    {
        Iterator<PIRLocation> &it = _spills.iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        _spills.destroy();
    }
    {
        Iterator<PIRLocation> &it = _temps.iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        _temps.destroy();
    }
    {
        Iterator<PIRStatement> &it = _statements.iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        _statements.destroy();
    }
}

void PIRMethod::init(MethodScope &scope) {
    scope.pir = this;
    this->_scope = &scope;
    MethodDeclNode &decl = *scope.getMethodDeclNode();
    
    if (!decl.global) {
        Type &thisType = *scope.getClassDeclNode()->instanceScope;
        this->_this = &env().create<PIRLocation, Type &, location_kind, int>(thisType, loc_this, 0);
        this->_thisTemp = newLocation(loc_temp, thisType);
        addMove(*this->_this, *this->_thisTemp);
    }
    
    {
        Iterator<VariableDeclNode> &it = decl.parameters.iterator();
        while (it.hasNext()) {
            VariableDeclNode &vDecl = it.next();
            VariableScope *vScope = vDecl.scope->isVariable();
            vScope->pir = newLocation(loc_param, *vDecl.resolvedType);
        }
        it.destroy();
    }
    
    {
        Iterator<TypeRefNode> &it = decl.returnTypes.iterator();
        while (it.hasNext()) {
            TypeRefNode &type = it.next();
            newLocation(loc_ret, *type.resolvedType);
        }
        it.destroy();
    }
}

PIRString &PIRMethod::getConstString(ConstCStringExprNode &value) {
    VariableScope *cstring = _scope->getClassDeclNode()->registerConstantCString(value);
    return env().create<PIRString, VariableScope&>(*cstring);
}

PIRLocation &PIRMethod::spillTemp(int idx) {
    PIRLocation &temp = *getTemp(idx);
    PIRLocation &spill = *newLocation(loc_spill, temp.type);
    // TODO: adjust and insert spill statements
    return spill;
}

void PIRMethod::addAsm(String & pasm) {
    _statements.add(env().create<PIRAsm, String&>(pasm));
}

void PIRMethod::addAssign(PIRValue &value, PIRLocation &dest) {
    if (value.isNull()) {
        if (!dest.type.isInstance()) {
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

void PIRMethod::addCall(PIRLocation &context, MethodScope &method, Collection<PIRLocation> &params, Collection<PIRLocation> &rets) {
    MethodDeclNode &decl = *method.getMethodDeclNode();
    // TODO: handle type conversion?
    if (method.getClassDeclNode()->instanceScope != &context.type) {
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
            if (vDecl.type.resolvedType != &loc.type) {
                error() << "incompatible parameter " << loc << " for method " << decl << "\n";
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
            if (&type != &loc.type) {
                error() << "incompatible return " << loc << " for method " << decl << "\n";
                return;
            }
        }
        typeIt.destroy();
        locIt.destroy();
    }
    _statements.add(env().create<PIRCall, PIRLocation &, MethodScope &, Collection<PIRLocation> &, Collection<PIRLocation> &>(context, method, params, rets));
}

void PIRMethod::addGet(PIRLocation &context, VariableScope &var, PIRLocation &dest) {
    VariableDeclNode &decl = *var.getVariableDeclNode();
    // TODO: handle type conversion?
    if (var.getClassDeclNode()->instanceScope != &context.type) {
        error() << "invalid context " << context << " for variable " << decl << "\n";
        return;
    }
    // TODO: handle type conversion?
    if (decl.resolvedType != &dest.type) {
        error() << "incompatible destination " << dest << " for variable " << decl << "\n";
        return;
    }
    _statements.add(env().create<PIRGet, PIRLocation &, VariableScope &, PIRLocation &>(context, var, dest));
}

void PIRMethod::addMove(PIRLocation &src, PIRLocation &dest) {
    // TODO: handle type conversion?
    if (&src.type != &dest.type) {
        error() << "incompatible types for move: " << src << " <-> " << dest << "\n";
        return;
    }
    _statements.add(env().create<PIRMove, PIRLocation&, PIRLocation&>(src, dest));
}

void PIRMethod::addReturn() {
    _statements.add(env().create<PIRReturn>());
}

void PIRMethod::addSet(PIRLocation &context, VariableScope &var, PIRLocation &src) {
    VariableDeclNode &decl = *var.getVariableDeclNode();
    // TODO: handle type conversion?
    if (var.getClassDeclNode()->instanceScope != &context.type) {
        error() << "invalid context " << context << " for variable " << decl << "\n";
        return;
    }
    // TODO: handle type conversion?
    if (decl.resolvedType != &src.type) {
        error() << "incompatible source " << src << " for variable " << decl << " " << decl.resolvedType << "\n";
        return;
    }
    _statements.add(env().create<PIRSet, PIRLocation &, VariableScope &, PIRLocation &>(context, var, src));
}

// private    
PIRLocation *PIRMethod::newLocation(location_kind kind, Type &type) {
    LinkedList<PIRLocation> *list = 0;
    switch (kind) {
        case loc_param:
            list = &_params;
            break;
        case loc_ret:
            list = &_rets;
            break;
        case loc_spill:
            list = &_spills;
            break;
        case loc_temp:
            list = &_temps;
            break;
    }
    if (!list) { return 0; }
    
    int idx = list->size();
    PIRLocation &ret = env().create<PIRLocation, Type &, location_kind, int>(type, kind, idx);
    list->add(ret);
    return &ret;
}
