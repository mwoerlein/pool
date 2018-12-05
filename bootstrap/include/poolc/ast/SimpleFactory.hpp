#ifndef POOLC_AST_SIMPLEFACTORY_HPP_LOCK
#define POOLC_AST_SIMPLEFACTORY_HPP_LOCK

#include "sys/Object.hpp"
#include "poolc/ast/nodes/ClassDefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"

// TODO: #2 replace factory with parser
class SimpleFactory: virtual public Object {
    private:
    ClassDefNode * objectDef;
    ClassDefNode * classDef;
    
    public:
    SimpleFactory(Environment &env, MemoryInfo &mi):Object(env, mi), objectDef(0), classDef(0) {}
    virtual ~SimpleFactory() {
        if (objectDef) { objectDef->destroy(); }
        if (classDef) { classDef->destroy(); }
    }
    
    virtual ClassDefNode * getDef(String &name) {
        if (name == "Object" || name == "/my/Object") {
            return &getObjectDef();
        } else if (name == "Class" || name == "/my/Class") {
            return &getClassDef();
        }
        return 0;
    }
    
    virtual ClassDefNode & getObjectDef() {
        if (!objectDef) {
            objectDef = &env().create<ClassDefNode>();
            ClassDefNode &cls = *objectDef;
            cls.name = "Object";
            cls.fullQualifiedName = "/my/Object";
            
            // supers
            cls.supers.add(getObjectDef());
            // vars
            // Runtime runtime
            {
                VariableDefNode &variable = env().create<VariableDefNode>();
                variable.name = "runtime";
                cls.variables.add(variable);
            }
            // methods
            // Class getClass()
            {
                MethodDefNode &method = env().create<MethodDefNode>();
                method.name = "getClass";
                method.body
                    << "    movl 12(%ebp), %eax    // @this (Type Object)\n"
                    << "    movl 4(%eax), %eax     // @this\n"
                    << "    movl (%eax), %eax      // @class desc\n"
                    << "    movl (%eax), %eax      // @class handle\n"
                    << "    movl %eax, 16(%ebp)    // return @class handle\n"
                ;
                cls.methods.add(method);
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(method);
                MethodRefNode &old = cls.methodRefs.set(method.name, ref);
                if (&old) { old.destroy(); }
                method.parent = ref.parent = &cls;
            }
            // int hash()
            {
                MethodDefNode &method = env().create<MethodDefNode>();
                method.name = "hash";
                method.body
                    << "    movl 12(%ebp), %eax    // @this (Type Object)\n"
                    << "    movl 4(%eax), %eax     // @this\n"
                    << "    movl %eax, 16(%ebp)    // return @this as hash\n"
                ;
                cls.methods.add(method);
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(method);
                MethodRefNode &old = cls.methodRefs.set(method.name, ref);
                if (&old) { old.destroy(); }
                method.parent = ref.parent = &cls;
            }
            // int equals(ANY)
            {
                MethodDefNode &method = env().create<MethodDefNode>();
                method.name = "equals";
                method.body
                    << "    movl 0, 20(%ebp)       // default return: false\n"
                    << "    movl 12(%ebp), %eax    // @this (Type Object)\n"
                    << "    movl 4(%eax), %eax     // @this\n"
                    << "    movl 16(%ebp), %ebx    // @obj (Type ANY)\n"
                    << "    movl 4(%ebx), %ebx     // @obj\n"
                    << "    cmpl %eax, %ebx\n"
                    << "    jne _come_ret\n"
                    << "    movl 1, 20(%ebp)       // return true\n"
                    << "_come_ret:\n"    
                ;
                cls.methods.add(method);
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(method);
                MethodRefNode &old = cls.methodRefs.set(method.name, ref);
                if (&old) { old.destroy(); }
                method.parent = ref.parent = &cls;
            }
            // Runtime rt()
            {
                MethodDefNode &method = env().create<MethodDefNode>();
                method.name = "rt";
                method.body
                    << "    movl 12(%ebp), %eax                         // @this (Type Object)\n"
                    << "    movl handle_Object_vars_Object(%eax), %ebx  // inst vars offset (Object)\n"
                    << "    addl 4(%eax), %ebx                          // @this.vars(Object)\n"
                    << "    movl Object_i_runtime(%ebx), %eax           // load @runtime (Type Runtime)\n"
                    << "    movl %eax, 16(%ebp)                         // return @runtime (Type Runtime)\n"
                ;
                cls.methods.add(method);
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(method);
                MethodRefNode &old = cls.methodRefs.set(method.name, ref);
                if (&old) { old.destroy(); }
                method.parent = ref.parent = &cls;
            }
            // void setRt(Runtime)
            {
                MethodDefNode &method = env().create<MethodDefNode>();
                method.name = "setRt";
                method.body
                    << "    movl 12(%ebp), %eax                         // @this (Type Object)\n"
                    << "    movl handle_Object_vars_Object(%eax), %ebx  // inst vars offset (Object)\n"
                    << "    addl 4(%eax), %ebx                          // @this.vars(Object)\n"
                    << "    movl 16(%ebp), %eax                         // arg @runtime (Type Runtime)\n"
                    << "    movl %eax, Object_i_runtime(%ebx)           // store @runtime (Type Runtime)\n"
                ;
                cls.methods.add(method);
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(method);
                MethodRefNode &old = cls.methodRefs.set(method.name, ref);
                if (&old) { old.destroy(); }
                method.parent = ref.parent = &cls;
            }
        }
        return *objectDef;
    }
    
    virtual ClassDefNode & getClassDef() {
        if (!classDef) {
            classDef = &env().create<ClassDefNode>();
            ClassDefNode &cls = *classDef;
            cls.name = "Class";
            cls.fullQualifiedName = "/my/Class";
            
            // supers
            cls.supers.add(getObjectDef());
            cls.supers.add(getClassDef());
            // vars
            // ClassDesc desc
            {
                VariableDefNode &variable = env().create<VariableDefNode>();
                variable.name = "desc";
                cls.variables.add(variable);
            }
            // inherited methods
            {
                Iterator<MethodRefNode> &it = getObjectDef().methodRefs.values();
                while (it.hasNext()) {
                    MethodRefNode &superRef = it.next();
                    MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(superRef.methodDef);
                    MethodRefNode &old = cls.methodRefs.set(superRef.methodDef.name, ref);
                    if (&old) { old.destroy(); }
                    ref.parent = &cls;
                }
                it.destroy();
            }
            // methods
            // ClassDesc getDesc()
            {
                MethodDefNode &method = env().create<MethodDefNode>();
                method.name = "getDesc";
                method.body
                    << "    movl 12(%ebp), %eax                      // @this (Type Class)\n"
                    << "    movl handle_Class_vars_Class(%eax), %ebx // inst vars offset (Class)\n"
                    << "    addl 4(%eax), %ebx                       // @this.vars(Class)\n"
                    << "    movl Class_i_desc(%ebx), %eax            // @class desc\n"
                    << "    movl %eax, 16(%ebp)                      // return @class desc\n"
                ;
                cls.methods.add(method);
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(method);
                MethodRefNode &old = cls.methodRefs.set(method.name, ref);
                if (&old) { old.destroy(); }
                method.parent = ref.parent = &cls;
            }
            // void setDesc(ClassDesc)
            {
                MethodDefNode &method = env().create<MethodDefNode>();
                method.name = "setDesc";
                method.body
                    << "    movl 12(%ebp), %eax                      // @this (Type Class)\n"
                    << "    movl handle_Class_vars_Class(%eax), %ebx // inst vars offset (Class)\n"
                    << "    addl 4(%eax), %ebx                       // @this.vars(Class)\n"
                    << "    movl 16(%ebp), %eax                      // param @class desc\n"
                    << "    movl %eax, Class_i_desc(%ebx)            // store @class desc\n"
                    << "    movl 12(%ebp), %ebx                      // @this (Type Class)\n"
                    << "    movl %ebx, (%eax)                        // store @this (Type Class) in class desc\n"
                ;
                cls.methods.add(method);
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(method);
                MethodRefNode &old = cls.methodRefs.set(method.name, ref);
                if (&old) { old.destroy(); }
                method.parent = ref.parent = &cls;
            }
            // CString getName()
            {
                MethodDefNode &method = env().create<MethodDefNode>();
                method.name = "getName";
                method.body
                    << "class_name_offset := 0x4 //(class_Class_name - class_Class_desc)\n"
                    << "    movl 12(%ebp), %eax                      // @this (Type Class)\n"
                    << "    movl handle_Class_vars_Class(%eax), %ebx // inst vars offset (Class)\n"
                    << "    addl 4(%eax), %ebx                       // @this.vars(Class)\n"
                    << "    movl Class_i_desc(%ebx), %eax            // @class desc\n"
                    << "    addl class_name_offset(%eax), %eax       // load reference to cstring\n"
                    << "    movl %eax, 16(%ebp)                      // return cstring-ref\n"
                ;
                cls.methods.add(method);
                MethodRefNode &ref = env().create<MethodRefNode, MethodDefNode&>(method);
                MethodRefNode &old = cls.methodRefs.set(method.name, ref);
                if (&old) { old.destroy(); }
                method.parent = ref.parent = &cls;
            }
            // inline pasm
            {
                cls.inlinePasm
                    << ".global class_vtabs_offset := 0x1c //(class_Class_vtabs - class_Class_desc)\n"
                    << ".global _cvte_size := 0x10 //(class_Class_vtabs_entry_Class - class_Class_vtabs_entry_Object)\n"
                    << ".global _cvte_cno := 0x4 //(class_Class_vtabs_entry_class_name - class_Class_vtabs_entry_Class)\n"
                    << ".global _cvte_cdo := 0x0 //(class_Class_vtabs_entry_class_desc - class_Class_vtabs_entry_Class)\n"
                    << ".global _cvte_vto := 0x8 //(class_Class_vtabs_entry_vtab_offset - class_Class_vtabs_entry_Class)\n"
                    << ".global _cvte_ho := 0xc //(class_Class_vtabs_entry_handle - class_Class_vtabs_entry_Class)\n"
                    << ".global class_instance_size_offset := 0x8 //(class_Class_instance_size - class_Class_desc)\n"
                    << ".global class_instance_tpl_offset_offset := 0xc //(class_Class_instance_tpl_offset - class_Class_desc)\n"
                    << ".global class_instance_Object_handle_offset := 0x10 //(class_Class_instance_Object_handle_offset - class_Class_desc)\n"
                    << ".global class_instance_class_handle_offset := 0x14 //(class_Class_instance_class_handle_offset - class_Class_desc)\n"
                ;
            }
        }
        return *classDef;
    }
};

#endif //POOLC_AST_SIMPLEFACTORY_HPP_LOCK

