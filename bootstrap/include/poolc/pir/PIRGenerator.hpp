#ifndef POOLC_PIR_PIRGENERATOR_HPP_LOCK
#define POOLC_PIR_PIRGENERATOR_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "sys/collection/LinkedList.hpp"

#include "poolc/pir/PIRLocation.hpp"
#include "poolc/pir/PIRMethod.hpp"
#include "poolc/pir/PIRValue.hpp"

class PIRGenerator: public Visitor {
    private:
    PIRMethod *curMethod;
    PIRValue *lastValue;
    LinkedList<PIRLocation> &lastLocations;
    
    public:
    PIRGenerator(Environment &env, MemoryInfo &mi);
    virtual ~PIRGenerator();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(ClassDeclNode & classDecl) override;
    virtual bool visit(MethodDeclNode & methodDecl) override;
    virtual bool visit(VariableDeclNode & variableDecl) override;
    
    virtual bool visit(BlockInstNode & block) override;
    virtual bool visit(ExpressionInstNode & exprInst) override;
    virtual bool visit(InlinePasmInstNode & pasmInst) override;
    virtual bool visit(ReturnInstNode & returnInst) override;
    virtual bool visit(VariableInitInstNode & variableInit) override;
    
    virtual bool visit(AssignmentExprNode & assignment) override;
    virtual bool visit(ConstCStringExprNode & constCString) override;
    virtual bool visit(ConstIntExprNode & constInt) override;
    virtual bool visit(MethodCallExprNode & methodCall) override;
    virtual bool visit(NullExprNode & constNull) override;
    virtual bool visit(ThisExprNode & constThis) override;
    virtual bool visit(VariableExprNode & variable) override;
};

#endif //POOLC_PIR_PIRGENERATOR_HPP_LOCK
