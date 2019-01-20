#ifndef POOLC_PIR_PIRGENERATOR_HPP_LOCK
#define POOLC_PIR_PIRGENERATOR_HPP_LOCK

#include "poolc/ast/Visitor.hpp"

#include "sys/collection/LinkedList.hpp"

#include "poolc/ast/TypeManager.hpp"

#include "poolc/pir/PIRBasicBlock.hpp"
#include "poolc/pir/PIRLocation.hpp"
#include "poolc/pir/PIRMethod.hpp"
#include "poolc/pir/PIRValue.hpp"

class PIRGenerator: public Visitor {
    private:
    TypeManager &types;
    PIRMethod *curMethod;
    PIRBasicBlock *curBlock;
    PIRValue *lastValue;
    LinkedList<PIRLocation> &lastLocations;
    
    virtual PIRLocation* asTemp(ExpressionNode &expr, Node &context);
    virtual void branch(ExpressionNode &expr, Node &context, PIRBasicBlock &trueBlock, PIRBasicBlock &falseBlock);
    
    public:
    PIRGenerator(Environment &env, MemoryInfo &mi, TypeManager &types);
    virtual ~PIRGenerator();
    
    virtual bool visit(TranslationUnitNode & translationUnit) override;

    virtual bool visit(ClassDeclNode & classDecl) override;
    virtual bool visit(MethodDeclNode & methodDecl) override;
    virtual bool visit(VariableDeclNode & variableDecl) override;
    
    virtual bool visit(BlockInstNode & block) override;
    virtual bool visit(ExpressionInstNode & exprInst) override;
    virtual bool visit(IfInstNode & ifInst) override;
    virtual bool visit(InlinePasmInstNode & pasmInst) override;
    virtual bool visit(ReturnInstNode & returnInst) override;
    virtual bool visit(VariableInitInstNode & variableInit) override;
    virtual bool visit(WhileInstNode & whileInst) override;
    
    virtual bool visit(ArithAssignmentExprNode & arithAssignment);
    virtual bool visit(ArithBinaryExprNode & arithBinary);
    virtual bool visit(ArithUnaryExprNode & arithUnary);
    virtual bool visit(AssignmentExprNode & assignment) override;
    virtual bool visit(ConstCStringExprNode & constCString) override;
    virtual bool visit(ConstIntExprNode & constInt) override;
    virtual bool visit(LogicalBinaryExprNode & logicalBinary) override;
    virtual bool visit(LogicalUnaryExprNode & logicalUnary) override;
    virtual bool visit(MethodCallExprNode & methodCall) override;
    virtual bool visit(NullExprNode & constNull) override;
    virtual bool visit(SignExprNode & sign);
    virtual bool visit(ThisExprNode & constThis) override;
    virtual bool visit(VariableExprNode & variable) override;
};

#endif //POOLC_PIR_PIRGENERATOR_HPP_LOCK
