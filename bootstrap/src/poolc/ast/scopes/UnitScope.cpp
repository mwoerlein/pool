#include "poolc/ast/scopes/UnitScope.hpp"

// public
UnitScope::UnitScope(Environment &env, MemoryInfo &mi, TranslationUnitNode & unit)
        :Scope(env, mi, 0), Object(env, mi),
         unit(unit) {
}
UnitScope::~UnitScope() {}
UnitScope * UnitScope::isUnit() { return this; }
TranslationUnitNode * UnitScope::getUnitNode() { return &unit; }
