#include "poolc/pir/PIRMethod.hpp"

#include "poolc/ast/nodes/declaration/ClassDeclNode.hpp"
#include "poolc/ast/nodes/declaration/MethodDeclNode.hpp"
#include "poolc/ast/nodes/declaration/VariableDeclNode.hpp"

#include "poolc/ast/scopes/ClassScope.hpp"
#include "poolc/ast/scopes/InstanceScope.hpp"

// public
PIRMethod::PIRMethod(Environment &env, MemoryInfo &mi)
        :Object(env, mi), LoggerAware(env, mi),
         _params(env.create<LinkedList<PIRLocation>>()),
         _rets(env.create<LinkedList<PIRLocation>>()),
         _spills(env.create<LinkedList<PIRLocation>>()),
         _temps(env.create<LinkedList<PIRLocation>>()),
         _blocks(env.create<LinkedList<PIRBasicBlock>>()),
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
        Iterator<PIRBasicBlock> &it = _blocks.iterator();
        while (it.hasNext()) {
            it.next().destroy();
        }
        it.destroy();
        _blocks.destroy();
    }
}

void PIRMethod::init(MethodScope &scope) {
    scope.pir = this;
    this->_scope = &scope;
    MethodDeclNode &decl = *scope.getMethodDeclNode();
    
    entry = &env().create<PIRBasicBlock, bb_kind, PIRMethod&, int>(bb_entry, *this, _blocks.size());
    _blocks.add(*entry);
    exit = &env().create<PIRBasicBlock, bb_kind, PIRMethod&, int>(bb_exit, *this, _blocks.size());
    _blocks.add(*exit);
            
    if (!decl.global) {
        Type &thisType = *scope.getInstance();
        this->_this = &env().create<PIRLocation, Type &, location_kind, int>(thisType, loc_this, 0);
        // TODO #11: treat all locations as "temps" until register allocations seperates frame and register locations
        //this->_thisTemp = newLocation(loc_temp, thisType);
        //entry->addMove(*this->_this, *this->_thisTemp);
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
        Iterator<Type> &it = decl.resolvedReturns.iterator();
        while (it.hasNext()) {
            newLocation(loc_ret, it.next());
        }
        it.destroy();
    }
}

PIRString &PIRMethod::getConstString(ConstCStringExprNode &value) {
    return env().create<PIRString, String&>(_scope->getClass()->stringId(value.value));
}

PIRLocation *PIRMethod::getZeroTemp(Type &type) {
    if (!_zeroTemp) {
        _zeroTemp = &newTemp(type);
        entry->addAssign(env().create<PIRInt, int>(0), *_zeroTemp);
    }
    return _zeroTemp;
}

PIRLocation *PIRMethod::getOneTemp(Type &type) {
    if (!_oneTemp) {
        _oneTemp = &newTemp(type);
        entry->addAssign(env().create<PIRInt, int>(1), *_oneTemp);
    }
    return _oneTemp;
}

PIRLocation &PIRMethod::spillTemp(int idx) {
    PIRLocation &temp = *getTemp(idx);
    PIRLocation &spill = *newLocation(loc_spill, temp.type);
    // TODO: adjust and insert spill statements
    return spill;
}

PIRBasicBlock &PIRMethod::newBasicBlock() {
    PIRBasicBlock &block = env().create<PIRBasicBlock, bb_kind, PIRMethod&, int>(bb_block, *this, _blocks.size());
    if (logger) { block.setLogger(*logger); }
    _blocks.add(block);
    return block;
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
