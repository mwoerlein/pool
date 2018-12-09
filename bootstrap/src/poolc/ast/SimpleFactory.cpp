#include "poolc/ast/SimpleFactory.hpp"

#include "poolc/ast/nodes/ClassRefNode.hpp"
#include "poolc/ast/nodes/MethodDefNode.hpp"
#include "poolc/ast/nodes/VariableDefNode.hpp"
#include "poolc/ast/nodes/CStringConstDefNode.hpp"
#include "poolc/ast/nodes/IntConstDefNode.hpp"
#include "poolc/ast/nodes/instruction/InlinePasmInstructionNode.hpp"

// public
SimpleFactory::SimpleFactory(Environment &env, MemoryInfo &mi)
        :Object(env, mi),
         objectDef(0),
         classDef(0),
         threadDef(0),
         runtimeDef(0),
         aDef(0),
         bDef(0) {
}
SimpleFactory::~SimpleFactory() {
    if (objectDef) { objectDef->destroy(); }
    if (classDef) { classDef->destroy(); }
    if (threadDef) { threadDef->destroy(); }
    if (runtimeDef) { runtimeDef->destroy(); }
    if (aDef) { aDef->destroy(); }
    if (bDef) { bDef->destroy(); }
}

ClassDefNode * SimpleFactory::getDef(String &name) {
    if (name == "Object" || name == "/my/Object") {
        return &getObjectDef();
    } else if (name == "Class" || name == "/my/Class") {
        return &getClassDef();
    } else if (name == "Thread" || name == "/my/Thread") {
        return &getThreadDef();
    } else if (name == "Runtime" || name == "/my/Runtime") {
        return &getRuntimeDef();
    } else if (name == "A" || name == "/my/A") {
        return &getADef();
    } else if (name == "B" || name == "/my/B") {
        return &getBDef();
    }
    return 0;
}

ClassDefNode & SimpleFactory::getObjectDef() {
    if (!objectDef) {
        objectDef = &env().create<ClassDefNode>();
        ClassDefNode &cls = *objectDef;
        cls.name = "Object";
        cls.fullQualifiedName = "/my/Object";
        
        // extends
        {}
        // vars
        // Runtime runtime
        {
            VariableDefNode &variable = env().create<VariableDefNode>();
            variable.name = "runtime";
            cls.variables.add(variable);
        }
        // consts
        // class header
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_inst_handle";
            constant.value = 0x4;
            cls.intConsts.add(constant);
        }
        // methods
        // Class getClass()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "getClass";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax    // @this (Type Object)\n"
                    << "    movl 4(%eax), %eax     // @this\n"
                    << "    movl (%eax), %eax      // @class desc\n"
                    << "    movl class_Object_ict_ch_inst_handle(%eax), %eax // @class handle\n"
                    << "    movl %eax, 16(%ebp)    // return @class handle\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // int hash()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "hash";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax    // @this (Type Object)\n"
                    << "    movl 4(%eax), %eax     // @this\n"
                    << "    movl %eax, 16(%ebp)    // return @this as hash\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // int equals(ANY)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "equals";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
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
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // Runtime rt()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "rt";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax                         // @this (Type Object)\n"
                    << "    movl handle_Object_vars_Object(%eax), %ebx  // inst vars offset (Object)\n"
                    << "    addl 4(%eax), %ebx                          // @this.vars(Object)\n"
                    << "    movl Object_i_runtime(%ebx), %eax           // load @runtime (Type Runtime)\n"
                    << "    movl %eax, 16(%ebp)                         // return @runtime (Type Runtime)\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void setRt(Runtime)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "setRt";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax                         // @this (Type Object)\n"
                    << "    movl handle_Object_vars_Object(%eax), %ebx  // inst vars offset (Object)\n"
                    << "    addl 4(%eax), %ebx                          // @this.vars(Object)\n"
                    << "    movl 16(%ebp), %eax                         // arg @runtime (Type Runtime)\n"
                    << "    movl %eax, Object_i_runtime(%ebx)           // store @runtime (Type Runtime)\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
    }
    return *objectDef;
}

ClassDefNode & SimpleFactory::getClassDef() {
    if (!classDef) {
        classDef = &env().create<ClassDefNode>();
        ClassDefNode &cls = *classDef;
        cls.name = "Class";
        cls.fullQualifiedName = "/my/Class";
        
        // extends
        {
            ClassRefNode &ref = env().create<ClassRefNode>();
            ref.name = "Object";
            cls.extends.add(ref);
        }
        // vars
        // ClassDesc desc
        {
            VariableDefNode &variable = env().create<VariableDefNode>();
            variable.name = "desc";
            cls.variables.add(variable);
        }
        // consts
        // class header
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_inst_handle";
            constant.value = 0x4;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_name";
            constant.value = 0x8;
            cls.intConsts.add(constant);
        }
        // methods
        // ClassDesc getDesc()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "getDesc";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax                      // @this (Type Class)\n"
                    << "    movl handle_Class_vars_Class(%eax), %ebx // inst vars offset (Class)\n"
                    << "    addl 4(%eax), %ebx                       // @this.vars(Class)\n"
                    << "    movl Class_i_desc(%ebx), %eax            // @class desc\n"
                    << "    movl %eax, 16(%ebp)                      // return @class desc\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void setDesc(ClassDesc)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "setDesc";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax                      // @this (Type Class)\n"
                    << "    movl handle_Class_vars_Class(%eax), %ebx // inst vars offset (Class)\n"
                    << "    addl 4(%eax), %ebx                       // @this.vars(Class)\n"
                    << "    movl 16(%ebp), %eax                      // param @class desc\n"
                    << "    movl %eax, Class_i_desc(%ebx)            // store @class desc\n"
                    << "    movl 12(%ebp), %ebx                      // @this (Type Class)\n"
                    << "    movl %ebx, class_Class_ict_ch_inst_handle(%eax)     // store @this (Type Class) in class desc\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // CString getName()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "getName";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax                      // @this (Type Class)\n"
                    << "    movl handle_Class_vars_Class(%eax), %ebx // inst vars offset (Class)\n"
                    << "    addl 4(%eax), %ebx                       // @this.vars(Class)\n"
                    << "    movl Class_i_desc(%ebx), %eax            // @class desc\n"
                    << "    addl class_Class_ict_ch_name(%eax), %eax       // load reference to cstring\n"
                    << "    movl %eax, 16(%ebp)                      // return cstring-ref\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
    }
    return *classDef;
}

ClassDefNode & SimpleFactory::getThreadDef() {
    if (!threadDef) {
        threadDef = &env().create<ClassDefNode>();
        ClassDefNode &cls = *threadDef;
        cls.name = "Thread";
        cls.fullQualifiedName = "/my/Thread";
        
        // extends
        {
            ClassRefNode &ref = env().create<ClassRefNode>();
            ref.name = "Object";
            cls.extends.add(ref);
        }
        // vars
        {}
        // consts
        {}
        // methods
        // void run()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "run";
            method.virt = true;
            cls.methods.add(method);
            method.parent = &cls;
        }
    }
    return *threadDef;
}

ClassDefNode & SimpleFactory::getRuntimeDef() {
    if (!runtimeDef) {
        runtimeDef = &env().create<ClassDefNode>();
        ClassDefNode &cls = *runtimeDef;
        cls.name = "Runtime";
        cls.fullQualifiedName = "/my/Runtime";
        
        // extends
        {
            ClassRefNode &ref = env().create<ClassRefNode>();
            ref.name = "Object";
            cls.extends.add(ref);
        }
        // vars
        // SysCall-Runtime syscall_runtime
        {
            VariableDefNode &variable = env().create<VariableDefNode>();
            variable.name = "syscall_runtime";
            cls.variables.add(variable);
        }
        // SysCall-Entry syscall_entry
        {
            VariableDefNode &variable = env().create<VariableDefNode>();
            variable.name = "syscall_entry";
            cls.variables.add(variable);
        }
        // consts
        {
            CStringConstDefNode &constant = env().create<CStringConstDefNode>();
            constant.name = "class";
            constant.value = "/my/Class";
            cls.consts.add(constant);
        }
        {
            CStringConstDefNode &constant = env().create<CStringConstDefNode>();
            constant.name = "thread";
            constant.value = "/my/Thread";
            cls.consts.add(constant);
        }
        // global print* constants
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "out"; // global alias for sps_out
            constant.value = 0;
            constant.global = true;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "err"; // global alias for sps_err
            constant.value = 1;
            constant.global = true;
            cls.intConsts.add(constant);
        }
        // SysCall constants
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "SysCall_allocate";
            constant.value = 1;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "SysCall_free";
            constant.value = 2;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "SysCall_find_class";
            constant.value = 3;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "SysCall_print";
            constant.value = 4;
            cls.intConsts.add(constant);
        }
        // SysCall print kinds
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "spk_char";
            constant.value = 0;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "spk_int";
            constant.value = 1;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "spk_hex";
            constant.value = 2;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "spk_string";
            constant.value = 3;
            cls.intConsts.add(constant);
        }
        // SysCall print streams
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "sps_out";
            constant.value = 0;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "sps_err";
            constant.value = 1;
            cls.intConsts.add(constant);
        }
        // class header
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_inst_handle";
            constant.value = 0x4;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_cts";
            constant.value = 0xc;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_tpl";
            constant.value = 0x14;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_tpl_size";
            constant.value = 0x18;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_tpl_obj_handle";
            constant.value = 0x1c;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "ch_tpl_cls_handle";
            constant.value = 0x20;
            cls.intConsts.add(constant);
        }
        // class tabs
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "cts_cdo";
            constant.value = 0x0;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "cts_vto";
            constant.value = 0x8;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "cts_ho";
            constant.value = 0xc;
            cls.intConsts.add(constant);
        }
        {
            IntConstDefNode &constant = env().create<IntConstDefNode>();
            constant.name = "cts_size";
            constant.value = 0x10;
            cls.intConsts.add(constant);
        }
        // methods
        // Runtime bootstrap(SysCall-Runtime, SysCall-Entry)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "bootstrap";
            method.scope = scope_class;
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    subl 12, %esp\n"
                    << "    pushad\n"
                    << "    \n"
                    << "    movl 0x0, 24(%ebp) // default result: NULL\n"
                    << "    movl 8(%ebp), %eax      // @class-desc \"Runtime\"\n"
                    << "    addl class_Runtime_so_ct_class, %eax\n"
                    << "    \n"
                    << "    pushl 0 // desc\n"
                    << "    pushl %eax        // \"Class\"\n"
                    << "    pushl class_Runtime_ict_SysCall_find_class\n"
                    << "    pushl %esp; pushl 16(%ebp); call 20(%ebp)\n"
                    << "    addl 16, %esp\n"
                    << "    popl -12(%ebp)  // store @class desc\n"
                    << "    \n"
                    << "    movl -12(%ebp), %edx\n"
                    << "    pushl 0     // info\n"
                    << "    pushl class_Runtime_ict_ch_tpl_size(%edx) // instance size\n"
                    << "    pushl class_Runtime_ict_SysCall_allocate;\n"
                    << "    pushl %esp; pushl 16(%ebp); call 20(%ebp)\n"
                    << "    addl 16, %esp\n"
                    << "    popl %eax   // return info\n"
                    << "    addl 0, %eax; jz _bs_return  // return NULL on allocate error\n"
                    << "    \n"
                    << "    movl -12(%ebp), %edx    // @class-desc \"Class\"\n"
                    << "    movl 8(%ebp), %ebx      // @class-desc \"Runtime\"\n"
                    << "    addl _cr_mo_call_entry, %ebx\n"
                    << "    call _crh_instantiate // %eax: @object-meminfo %ebx: @_call_entry %edx: @class-desc, return %edi: @object (Type Object) %esi: @object (Type <class>)\n"
                    << "	movl %edi, -8(%ebp) // store @Class (Type Object)\n"
                    << "	\n"
                    << "    pushl -12(%ebp)\n"
                    << "    pushl %esi; pushl Class_m_setDesc; call (%esi)\n"
                    << "	addl 12, %esp\n"
                    << "	\n"
                    << "    movl 8(%ebp), %edx      // @class-desc \"Runtime\"\n"
                    << "    pushl 0     // info\n"
                    << "    pushl class_Runtime_ict_ch_tpl_size(%edx) // instance size\n"
                    << "    pushl class_Runtime_ict_SysCall_allocate;\n"
                    << "    pushl %esp; pushl 16(%ebp); call 20(%ebp)\n"
                    << "    addl 16, %esp\n"
                    << "    popl %eax   // return info\n"
                    << "    addl 0, %eax; jz _bs_return  // return NULL on allocate error\n"
                    << "    \n"
                    << "    movl 8(%ebp), %edx      // @class-desc \"Runtime\"\n"
                    << "    movl 8(%ebp), %ebx      // @class-desc \"Runtime\"\n"
                    << "    addl _cr_mo_call_entry, %ebx\n"
                    << "    call _crh_instantiate // %eax: @object-meminfo %ebx: @_call_entry %edx: @class-desc, return %edi: @object (Type Object) %esi: @object (Type <class>)\n"
                    << "    \n"
                    << "    pushl %esi\n"
                    << "    pushl %edi; pushl Object_m_setRt; call (%edi)\n"
                    << "	addl 12, %esp\n"
                    << "    \n"
                    << "    pushl 20(%ebp) // @syscall-entry\n"
                    << "    pushl 16(%ebp) // @syscall-runtime\n"
                    << "    pushl %esi; pushl Runtime_m_initSysCall; call (%esi)\n"
                    << "	addl 16, %esp\n"
                    << "	\n"
                    << "	movl -8(%ebp), %edi // load @Class (Type Object)\n"
                    << "    pushl %esi\n"
                    << "    pushl %edi; pushl Object_m_setRt; call (%edi)\n"
                    << "	addl 12, %esp\n"
                    << "    \n"
                    << "    movl 8(%ebp), %eax      // @class-desc \"Runtime\"\n"
                    << "    addl class_Runtime_so_ct_class, %eax\n"
                    << "    subl 4, %esp  # return value of createInstance\n"
                    << "    pushl %eax // @classname\n"
                    << "    pushl %esi; pushl Runtime_m_createInstance; call (%esi)\n"
                    << "	addl 12, %esp\n"
                    << "    popl %eax   // @class (Type Class)\n"
                    << "    addl 0, %eax; jz _crmci_return  // return NULL if class could not be initialized\n"
                    << "    \n"
                    << "    pushl 8(%ebp)      // @class-desc \"Runtime\"\n"
                    << "    pushl %eax; pushl Class_m_setDesc; call (%eax)\n"
                    << "	addl 12, %esp\n"
                    << "	\n"
                    << "    movl %esi, 24(%ebp) // store @runtime (Type Runtime) as result\n"
                    << "_bs_return:\n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
            cls.bootstrap = &method;
        }
        // void initSysCall(SysCall-Runtime, SysCall-Entry)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "initSysCall";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax                           // @this (Type Runtime)\n"
                    << "    movl handle_Runtime_vars_Runtime(%eax), %ebx  // inst vars offset (Runtime)\n"
                    << "    addl 4(%eax), %ebx                            // @this.vars(Runtime)\n"
                    << "    movl 16(%ebp), %eax                           // arg @syscall-runtime\n"
                    << "    movl %eax, Runtime_i_syscall_runtime(%ebx)    // store @syscall-runtime\n"
                    << "    movl 20(%ebp), %eax                           // arg @syscall-entry\n"
                    << "    movl %eax, Runtime_i_syscall_entry(%ebx)      // store @syscall-entry\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // ClassDesc getClassDesc(CString)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "getClassDesc";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushad\n"
                    << "    \n"
                    << "    movl 12(%ebp), %eax                           // @this (Type Runtime)\n"
                    << "    movl handle_Runtime_vars_Runtime(%eax), %ebx  // inst vars offset (Runtime)\n"
                    << "    addl 4(%eax), %ebx                            // @this.vars(Runtime)\n"
                    << "    \n"
                    << "    pushl 0         // desc\n"
                    << "    pushl 16(%ebp)  // @classname\n"
                    << "    pushl class_Runtime_ict_SysCall_find_class;\n"
                    << "    pushl %esp; pushl Runtime_i_syscall_runtime(%ebx); call Runtime_i_syscall_entry(%ebx)\n"
                    << "    addl 16, %esp\n"
                    << "    popl 20(%ebp)   // return @class desc\n"
                    << "    \n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // MemInfo allocate(int)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "allocate";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushad\n"
                    << "    \n"
                    << "    movl 12(%ebp), %eax                           // @this (Type Runtime)\n"
                    << "    movl handle_Runtime_vars_Runtime(%eax), %ebx  // inst vars offset (Runtime)\n"
                    << "    addl 4(%eax), %ebx                            // @this.vars(Runtime)\n"
                    << "    \n"
                    << "    pushl 0         // info\n"
                    << "    pushl 16(%ebp)  // param size\n"
                    << "    pushl class_Runtime_ict_SysCall_allocate;\n"
                    << "    pushl %esp; pushl Runtime_i_syscall_runtime(%ebx); call Runtime_i_syscall_entry(%ebx)\n"
                    << "    addl 16, %esp\n"
                    << "    popl 20(%ebp)   // return info\n"
                    << "    \n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void free(MemInfo)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "free";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushad\n"
                    << "    \n"
                    << "    movl 12(%ebp), %eax                           // @this (Type Runtime)\n"
                    << "    movl handle_Runtime_vars_Runtime(%eax), %ebx  // inst vars offset (Runtime)\n"
                    << "    addl 4(%eax), %ebx                            // @this.vars(Runtime)\n"
                    << "    \n"
                    << "    pushl 16(%ebp)  // param info\n"
                    << "    pushl 0         // size\n"
                    << "    pushl class_Runtime_ict_SysCall_free;\n"
                    << "    pushl %esp; pushl Runtime_i_syscall_runtime(%ebx); call Runtime_i_syscall_entry(%ebx)\n"
                    << "    addl 20, %esp\n"
                    << "    \n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void printChar(int, int)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "printChar";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushad\n"
                    << "    \n"
                    << "    movl 12(%ebp), %eax                           // @this (Type Runtime)\n"
                    << "    movl handle_Runtime_vars_Runtime(%eax), %ebx  // inst vars offset (Runtime)\n"
                    << "    addl 4(%eax), %ebx                            // @this.vars(Runtime)\n"
                    << "    \n"
                    << "    pushl 20(%ebp)  // param c\n"
                    << "    pushl class_Runtime_ict_spk_char // kind\n"
                    << "    pushl 16(%ebp)  // param stream\n"
                    << "    pushl class_Runtime_ict_SysCall_print;\n"
                    << "    pushl %esp; pushl Runtime_i_syscall_runtime(%ebx); call Runtime_i_syscall_entry(%ebx)\n"
                    << "    addl 24, %esp\n"
                    << "    \n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void printString(CString, int)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "printString";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushad\n"
                    << "    \n"
                    << "    movl 12(%ebp), %eax                           // @this (Type Runtime)\n"
                    << "    movl handle_Runtime_vars_Runtime(%eax), %ebx  // inst vars offset (Runtime)\n"
                    << "    addl 4(%eax), %ebx                            // @this.vars(Runtime)\n"
                    << "    \n"
                    << "    pushl 20(%ebp)    // param s\n"
                    << "    pushl class_Runtime_ict_spk_string // kind\n"
                    << "    pushl 16(%ebp)    // param stream\n"
                    << "    pushl class_Runtime_ict_SysCall_print;\n"
                    << "    pushl %esp; pushl Runtime_i_syscall_runtime(%ebx); call Runtime_i_syscall_entry(%ebx)\n"
                    << "    addl 24, %esp\n"
                    << "    \n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void printInt(int, int)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "printInt";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushad\n"
                    << "    \n"
                    << "    movl 12(%ebp), %eax                           // @this (Type Runtime)\n"
                    << "    movl handle_Runtime_vars_Runtime(%eax), %ebx  // inst vars offset (Runtime)\n"
                    << "    addl 4(%eax), %ebx                            // @this.vars(Runtime)\n"
                    << "    \n"
                    << "    pushl 20(%ebp)  // param i\n"
                    << "    pushl class_Runtime_ict_spk_int  // kind\n"
                    << "    pushl 16(%ebp)  // param stream\n"
                    << "    pushl class_Runtime_ict_SysCall_print;\n"
                    << "    pushl %esp; pushl Runtime_i_syscall_runtime(%ebx); call Runtime_i_syscall_entry(%ebx)\n"
                    << "    addl 24, %esp\n"
                    << "    \n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void printHex(int, int)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "printHex";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushad\n"
                    << "    \n"
                    << "    movl 12(%ebp), %eax                           // @this (Type Runtime)\n"
                    << "    movl handle_Runtime_vars_Runtime(%eax), %ebx  // inst vars offset (Runtime)\n"
                    << "    addl 4(%eax), %ebx                            // @this.vars(Runtime)\n"
                    << "    \n"
                    << "    pushl 20(%ebp)  // param i\n"
                    << "    pushl class_Runtime_ict_spk_hex  // kind\n"
                    << "    pushl 16(%ebp)  // param stream\n"
                    << "    pushl class_Runtime_ict_SysCall_print;\n"
                    << "    pushl %esp; pushl Runtime_i_syscall_runtime(%ebx); call Runtime_i_syscall_entry(%ebx)\n"
                    << "    addl 24, %esp\n"
                    << "    \n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void destroyInstance(ANY)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "destroyInstance";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 16(%ebp), %eax       // @obj (Type ANY)\n"
                    << "    movl 4(%eax), %eax        // @obj\n"
                    << "    movl 12(%ebp), %ebx       // @this (Type Runtime)\n"
                    << "    \n"
                    << "    pushl 4(%eax)             // @obj meminfo\n"
                    << "    pushl %ebx; pushl Runtime_m_free; call (%ebx)\n"
                    << "    addl 12, %esp\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // ANY as(CString, ANY)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "as";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushl %ecx\n"
                    << "    pushl %esi\n"
                    << "_crma_start:\n"
                    << "    movl 0, 24(%ebp)    // not-found default handle: NULL\n"
                    << "    movl 12(%ebp), %esi // @this (Type Runtime)\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of getClassDesc\n"
                    << "    pushl 20(%ebp)  // param @classname\n"
                    << "    pushl %esi; pushl Runtime_m_getClassDesc; call (%esi)\n"
                    << "	addl 12, %esp\n"
                    << "    popl %ecx       // @class-desc\n"
                    << "    addl 0, %ecx; jz _crma_return   // return NULL if class not exists\n"
                    << "   \n"
                    << "    movl 16(%ebp), %eax             // @obj (Type ANY)\n"
                    << "    movl 4(%eax), %ebx              // @obj\n"
                    << "    movl (%ebx), %eax               // @obj-class desc\n"
                    << "    addl class_Runtime_ict_ch_cts(%eax), %eax // @obj-class vtabs entry\n"
                    << "_crma_loop:\n"
                    << "    cmpl (%eax), %ecx\n"
                    << "    je _crma_found\n"
                    << "    addl class_Runtime_ict_cts_size, %eax\n"
                    << "    cmpl 0, (%eax)\n"
                    << "    je _crma_return\n"
                    << "    jmp _crma_loop\n"
                    << "_crma_found:\n"
                    << "    addl class_Runtime_ict_cts_ho(%eax), %ebx\n"
                    << "    movl %ebx, 24(%ebp) // return correct handle\n"
                    << "_crma_return:\n"
                    << "    popl %esi\n"
                    << "    popl %ecx\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // Thread createThread(CString)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "createThread";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushl %ecx\n"
                    << "    pushl %esi\n"
                    << "_crmct_start:\n"
                    << "    movl 0, 20(%ebp)          // default handle: NULL\n"
                    << "    movl 12(%ebp), %esi       // @this (Type Runtime)\n"
                    << "    \n"
                ;
                method.body.add(pasm);
            }
            // createInstance
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    subl 4, %esp  # return value of createInstance\n"
                    << "    pushl 16(%ebp)  // param @classname\n"
                    << "    pushl %esi; pushl Runtime_m_createInstance; call (%esi)\n"
                    << "	addl 12, %esp\n"
                    << "    popl %ecx; // @instance (type <classname>)\n"
                    << "    addl 0, %ecx; jz _crmct_return // break if not instantiated\n"
                    << "    \n"
                ;
                method.body.add(pasm);
            }
            // cast to Thread
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 8(%ebp), %eax      // @class-desc \"Runtime\"\n"
                    << "    addl class_Runtime_so_ct_thread, %eax\n"
                    << "    subl 4, %esp  # return value of as\n"
                    << "    pushl %eax\n"
                    << "    pushl %ecx\n"
                    << "    pushl %esi; pushl Runtime_m_as; call (%esi)\n"
                    << "	addl 16, %esp\n"
                    << "    popl %eax; // @instance (type \"Thread\")\n"
                    << "    addl 0, %eax; jz _crmct_cleanup // destroy instance if not a thread\n"
                    << "    \n"
                ;
                method.body.add(pasm);
            }
            // return or cleanup
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl %eax, 20(%ebp)  // return @instance (type \"Thread\")\n"
                    << "	jmp _crmct_return\n"
                    << "    \n"
                    << "_crmct_cleanup:\n"
                    << "    pushl %ecx\n"
                    << "    pushl %esi; pushl Runtime_m_destroyInstance; call (%esi)\n"
                    << "	addl 12, %esp\n"
                    << "_crmct_return:\n"
                    << "    popl %esi\n"
                    << "    popl %ecx\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // ANY createInstance(CString)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "createInstance";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    \n"
                    << "    pushad\n"
                    << "_crmci_start:\n"
                    << "    movl 0, 20(%ebp)          // default handle: NULL\n"
                    << "    movl 12(%ebp), %esi       // @this (Type Runtime)\n"
                    << "    \n"
                ;
                method.body.add(pasm);
            }
            // getClassDesc
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    subl 4, %esp  # return value of getClassDesc\n"
                    << "    pushl 16(%ebp)  // param @classname\n"
                    << "    pushl %esi; pushl Runtime_m_getClassDesc; call (%esi)\n"
                    << "	addl 12, %esp\n"
                    << "    popl %edx       // @class-desc\n"
                    << "    addl 0, %edx; jz _crmci_return  // return NULL if class not exists\n"
                    << "    \n"
                ;
                method.body.add(pasm);
            }
            // if (!Class instance exist)
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    cmpl 0, class_Runtime_ict_ch_inst_handle(%edx)\n"
                    << "    jnz _crmci_instantiate // class already initialized\n"
                    << "    \n"
                ;
                method.body.add(pasm);
            }
            // create Class instance
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 8(%ebp), %eax      // @class-desc \"Runtime\"\n"
                    << "    addl class_Runtime_so_ct_class, %eax\n"
                    << "    subl 4, %esp  # return value of createInstance\n"
                    << "    pushl %eax // @classname\n"
                    << "    pushl %esi; pushl Runtime_m_createInstance; call (%esi)\n"
                    << "	addl 12, %esp\n"
                    << "    popl %eax   // @class (Type Class)\n"
                    << "    addl 0, %eax; jz _crmci_return  // return NULL if class could not be initialized\n"
                    << "    \n"
                ;
                method.body.add(pasm);
            }
            // setDescriptor
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushl %edx                      // @class-desc\n"
                    << "    pushl %eax; pushl Class_m_setDesc; call (%eax)\n"
                    << "	addl 12, %esp\n"
                    << "    \n"
                ;
                method.body.add(pasm);
            }
            // endif
            // allocate memory for instance
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "_crmci_instantiate:\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of allocate\n"
                    << "    pushl class_Runtime_ict_ch_tpl_size(%edx) // instance size\n"
                    << "    pushl %esi; pushl Runtime_m_allocate; call (%esi)\n"
                    << "	addl 12, %esp\n"
                    << "    popl %eax                       // @object-meminfo\n"
                    << "    addl 0, %eax; jz _crmci_return\n"
                ;
                method.body.add(pasm);
            }
            // call _crh_instantiate AND return correct handle
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    \n"
                    << "    pushl %esi // @Runtime for later setRt\n"
                    << "    movl 8(%ebp), %ebx      // @class-desc \"Runtime\"\n"
                    << "    addl _cr_mo_call_entry, %ebx\n"
                    << "    call _crh_instantiate // %eax: @object-meminfo %ebx: @_call_entry %edx: @class-desc, return %edi: @object (Type Object) %esi: @object (Type <class>)\n"
                    << "    addl 0, %esi; jz _crmci_return  // return NULL if instance could not be generated\n"
                    << "    movl %esi, 20(%ebp)             // return correct handle\n"
                    << "    pushl %edi; pushl Object_m_setRt; call (%edi)\n"
                    << "	addl 12, %esp\n"
                    << "    \n"
                    << "_crmci_return:\n"
                    << "    popad\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void _crh_instantiate()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "_crh_instantiate";
            method.naked = true;
            method.scope = scope_class;
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "_crh_instantiate: // %eax: @object-meminfo %ebx: @_call_entry %edx: @Class-desc, return %edi: @object (Type Object) %esi: @object (Type <class>)\n"
                    << "    movl (%eax), %edi   // @object\n"
                    << "    movl %edx, %esi\n"
                    << "    addl class_Runtime_ict_ch_tpl(%edx), %esi // @instance tpl\n"
                    << "    movl class_Runtime_ict_ch_tpl_size(%edx), %ecx // instance size\n"
                    << "    .byte 0xf3; .byte 0xa4 #// rep movsb // copy template to object\n"
                    << "    \n"
                    << "    movl (%eax), %edi   // @object\n"
                    << "    movl %edx, (%edi)   // store @class desc in instance\n" 
                    << "    movl %eax, 4(%edi)  // store @meminfo in instance\n"
                    << "    \n"
                    << "    movl %edx, %eax                     // @obj-class desc\n"
                    << "    addl class_Runtime_ict_ch_cts(%eax), %eax // @obj-class vtabs entry\n"
                    << "_crhi_loop:\n"
                    << "    movl class_Runtime_ict_cts_ho(%eax), %esi\n"
                    << "    movl class_Runtime_ict_cts_vto(%eax), %ecx\n"
                    << "    movl %ebx, (%edi, %esi)         // store @call-entry in handle\n"
                    << "    movl %edi, 4(%edi, %esi)        // store @object in handle\n"
                    << "    movl %ecx, 8(%edi, %esi)        // store vtab-offset in handle\n"
                    << "    addl class_Runtime_ict_cts_size, %eax\n"
                    << "    cmpl 0, (%eax)\n"
                    << "    jne _crhi_loop\n"
                    << "    \n"
                    << "    movl %edi, %esi\n"
                    << "    addl class_Runtime_ict_ch_tpl_obj_handle(%edx), %edi // @object (Type Object)\n"
                    << "    addl class_Runtime_ict_ch_tpl_cls_handle(%edx), %esi // @object (Type <class>)\n"
                    << "    ret\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void _call_entry()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "_call_entry";
            method.naked = true;
            method.scope = scope_class;
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "_cr_mo_call_entry := (_call_entry - class_Runtime_desc)\n"
                    << "_call_entry:\n"
                    << "	pushl %ecx\n"
                    << "	movl 12(%esp), %ebx	        # load object handle\n"
                    << "	movl 4(%ebx), %ecx	        # get object\n"
                    << "	movl 0(%ecx), %eax          # get class-desc\n"
                    << "	addl 8(%ebx), %eax          # get vtab\n"
                    << "	addl 8(%esp), %eax	        # get vtab-entry by adding method-offset number\n"
                    << "	movl 0(%ecx), %ebx	        # get class-desc\n"
                    << "	addl 4(%eax), %ebx          # get method-vtabs-entry\n"
                    << "	addl class_Runtime_ict_cts_ho(%ebx), %ecx   # compute method-@this\n"
                    << "	movl %ecx, 12(%esp)         # store method-@this\n"
                    << "	movl class_Runtime_ict_cts_cdo(%ebx), %ebx  # get method-class-desc\n"
                    << "	movl %ebx, 8(%esp)          # store method-class-desc\n"
                    << "	addl 0(%eax), %ebx          # compute method-addr\n"
                    << "	popl %ecx\n"
                    << "	jmp %ebx                    # goto method\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
    }
    return *runtimeDef;
}

ClassDefNode & SimpleFactory::getADef() {
    if (!aDef) {
        aDef = &env().create<ClassDefNode>();
        ClassDefNode &cls = *aDef;
        cls.name = "A";
        cls.fullQualifiedName = "/my/A";
        
        // extends
        {
            ClassRefNode &ref = env().create<ClassRefNode>();
            ref.name = "Object";
            cls.extends.add(ref);
        }
        // vars
        // int column
        {
            VariableDefNode &variable = env().create<VariableDefNode>();
            variable.name = "column";
            cls.variables.add(variable);
        }
        // int row
        {
            VariableDefNode &variable = env().create<VariableDefNode>();
            variable.name = "row";
            cls.variables.add(variable);
        }
        // consts
        {
            CStringConstDefNode &constant = env().create<CStringConstDefNode>();
            constant.name = "test";
            constant.value = " Test\n";
            cls.consts.add(constant);
        }
        // methods
        // void init(int, int)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "init";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax               // @this (Type A)\n"
                    << "    movl handle_A_vars_A(%eax), %ebx  // inst vars offset (A)\n"
                    << "    addl 4(%eax), %ebx                // @this.vars(A)\n"
                    << "    \n"
                    << "    movl 16(%esp), %eax         // param row\n"
                    << "    movl %eax, A_i_row(%ebx)    // set this.row\n"
                    << "    movl 20(%esp), %eax         // param column\n"
                    << "    movl %eax, A_i_column(%ebx) // set this.column\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // int getRow()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "getRow";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax               // @this (Type A)\n"
                    << "    movl handle_A_vars_A(%eax), %ebx  // inst vars offset (A)\n"
                    << "    addl 4(%eax), %ebx                // @this.vars(A)\n"
                    << "    \n"
                    << "    movl A_i_row(%ebx), %eax // row\n"
                    << "    movl %eax, 16(%ebp)      // return row\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void test(int)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "test";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushl %ecx\n"
                    << "    pushl %edx\n"
                    << "    \n"
                    << "    movl 12(%ebp), %ecx               // @this (Type A)\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of rt\n"
                    << "    pushl %ecx; pushl A_m_rt; call (%ecx)\n"
                    << "	addl 8, %esp\n"
                    << "    popl %edx   # Runtime (Type Runtime)\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of getClass\n"
                    << "    pushl %ecx; pushl A_m_getClass; call (%ecx)\n"
                    << "	addl 8, %esp\n"
                    << "    popl %eax // handle_Class_(A|B) (Type Class)\n"
                    << "/*\n"
                    << "    subl 4, %esp  # return value of getClass\n"
                    << "    pushl %eax; pushl Class_m_getClass; call (%eax)\n"
                    << "	addl 8, %esp\n"
                    << "    popl %eax // handle_Class_Class (Type Class)\n"
                    << "*/\n"
                    << "    subl 4, %esp  # return value of getName\n"
                    << "    pushl %eax; pushl Class_m_getName; call (%eax)\n"
                    << "	addl 8, %esp\n"
                    << "    popl %eax // name cstring ref\n"
                    << "    \n"
                    << "    pushl %eax; pushl Runtime_c_out\n"
                    << "    pushl %edx; pushl Runtime_m_printString; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl 0x20; pushl Runtime_c_out // ' '\n"
                    << "    pushl %edx; pushl Runtime_m_printChar; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    movl handle_A_vars_A(%ecx), %ebx  // inst vars offset (A)\n"
                    << "    addl 4(%ecx), %ebx                // @this.vars(A)\n"
                    << "    push A_i_column(%ebx); pushl Runtime_c_out // column\n"
                    << "    pushl %edx; pushl Runtime_m_printInt; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl 0x20; pushl Runtime_c_out // ' '\n"
                    << "    pushl %edx; pushl Runtime_m_printChar; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl 16(%ebp); pushl Runtime_c_out // row\n"
                    << "    pushl %edx; pushl Runtime_m_printInt; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    movl 8(%ebp), %eax      // @class-desc \"A\"\n"
                    << "    addl class_A_so_ct_test, %eax\n"
                    << "    pushl %eax; pushl Runtime_c_err\n"
                    << "    pushl %edx; pushl Runtime_m_printString; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    popl %ecx\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
    }
    return *aDef;
}

ClassDefNode & SimpleFactory::getBDef() {
    if (!bDef) {
        bDef = &env().create<ClassDefNode>();
        ClassDefNode &cls = *bDef;
        cls.name = "B";
        cls.fullQualifiedName = "/my/B";
        
        // extends
        {
            ClassRefNode &ref = env().create<ClassRefNode>();
            ref.name = "Thread";
            cls.extends.add(ref);
        }
        {
            ClassRefNode &ref = env().create<ClassRefNode>();
            ref.name = "A";
            cls.extends.add(ref);
        }
        // vars
        {}
        // consts
        {
            CStringConstDefNode &constant = env().create<CStringConstDefNode>();
            constant.name = "doit";
            constant.value = "OMG! It work's! ";
            cls.consts.add(constant);
        }
        // methods
        // void run()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "run";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushl %ecx\n"
                    << "    pushl %edx\n"
                    << "    pushl %edi\n"
                    << "    \n"
                    << "    movl 12(%ebp), %ecx # @this (Type B)\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of rt\n"
                    << "    pushl %ecx; pushl B_m_rt; call (%ecx)\n"
                    << "	addl 8, %esp\n"
                    << "    popl %edi   # Runtime(Type Runtime)\n"
                    << "    \n"
                    << "    pushl 20\n"
                    << "    pushl 2\n"
                    << "    pushl %ecx; pushl B_m_init; call (%ecx)\n"
                    << "	addl 16, %esp\n"
                    << "	\n"
                    << "    movl 8(%ebp), %eax          // @class-desc \"B\"\n"
                    << "    addl class_B_so_cn_A, %eax  // \"/my/A\"\n"
                    << "    subl 4, %esp  # return value of createInstance\n"
                    << "    pushl %eax\n"
                    << "    pushl %edi; pushl Runtime_m_createInstance; call (%edi)\n"
                    << "	addl 12, %esp\n"
                    << "    popl %edx; // inst_A (type A)\n"
                    << "    \n"
                    << "    pushl 5\n"
                    << "    pushl 2\n"
                    << "    pushl %edx; pushl A_m_init; call (%edx)\n"
                    << "	addl 16, %esp\n"
                    << "    \n"
                    << "    pushl %edx\n"    
                    << "    pushl %ecx; pushl B_m_doIt; call (%ecx)\n"
                    << "	addl 12, %esp\n"
                    << "    \n"
                    << "    pushl %edx\n"
                    << "    pushl %edi; pushl Runtime_m_destroyInstance; call (%edi)\n"
                    << "	addl 12, %esp\n"
                    << "    \n"
                    << "    popl %edi\n"
                    << "    popl %edx\n"
                    << "    popl %ecx\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // void doIt(A)
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "doIt";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    pushl %ecx\n"
                    << "    pushl %edx\n"
                    << "    \n"
                    << "    movl 12(%ebp), %ecx # @this (Type B)\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of rt\n"
                    << "    pushl %ecx; pushl B_m_rt; call (%ecx)\n"
                    << "	addl 8, %esp\n"
                    << "    popl %edx   # Runtime(Type Runtime)\n"
                    << "    \n"
                    << "    pushl 0x40; pushl Runtime_c_err // '@'\n"
                    << "    pushl %edx; pushl Runtime_m_printChar; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    movl 8(%ebp), %eax         // @class-desc \"B\"\n"
                    << "    addl class_B_so_ct_doit, %eax // \"DoIt \"\n"
                    << "    pushl %eax; pushl Runtime_c_out\n"
                    << "    pushl %edx; pushl Runtime_m_printString; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of equals\n"
                    << "    pushl 12(%ebp) # @this (Type B)\n"
                    << "    pushl %ecx; pushl B_m_equals; call (%ecx)\n"
                    << "    addl 12, %esp\n"
                    << "    popl %eax\n"
                    << "    \n"
                    << "    pushl %eax; pushl Runtime_c_out\n"
                    << "    pushl %edx; pushl Runtime_m_printInt; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl 0x20; pushl Runtime_c_out // ' '\n"
                    << "    pushl %edx; pushl Runtime_m_printChar; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of equals\n"
                    << "    pushl 16(%ebp) # param @a (Type A)\n"
                    << "    pushl %ecx; pushl B_m_equals; call (%ecx)\n"
                    << "    addl 12, %esp\n"
                    << "    popl %eax\n"
                    << "    \n"
                    << "    pushl %eax; pushl Runtime_c_out\n"
                    << "    pushl %edx; pushl Runtime_m_printInt; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of allocate\n"
                    << "    pushl 0x124\n"
                    << "    pushl %edx; pushl Runtime_m_allocate; call (%edx)\n"
                    << "	addl 12, %esp\n"
                    << "    popl %esi\n"
                    << "    \n"
                    << "    pushl 0x20; pushl Runtime_c_out // ' '\n"
                    << "    pushl %edx; pushl Runtime_m_printChar; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl %esi; pushl Runtime_c_out\n"
                    << "    pushl %edx; pushl Runtime_m_printHex; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl 0x20; pushl Runtime_c_out // ' '\n"
                    << "    pushl %edx; pushl Runtime_m_printChar; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl (%esi); pushl Runtime_c_out\n"
                    << "    pushl %edx; pushl Runtime_m_printHex; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl 0x20; pushl Runtime_c_out // ' '\n"
                    << "    pushl %edx; pushl Runtime_m_printChar; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl 4(%esi); pushl Runtime_c_out\n" 
                    << "    pushl %edx; pushl Runtime_m_printHex; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl 0xa; pushl Runtime_c_out // '/n'\n"
                    << "    pushl %edx; pushl Runtime_m_printChar; call (%edx)\n"
                    << "    addl 16, %esp\n"
                    << "    \n"
                    << "    pushl %esi\n" 
                    << "    pushl %edx; pushl Runtime_m_free; call (%edx)\n"
                    << "    addl 12, %esp\n"
                    << "    \n"
                    << "    subl 4, %esp  # return value of getRow\n"
                    << "    pushl %ecx; pushl B_m_getRow; call (%ecx)\n"
                    << "	addl 8, %esp\n"
                    << "    popl %eax\n"
                    << "    \n"
                    << "    pushl %eax     # row\n"
                    << "    pushl %ecx; pushl B_m_test; call (%ecx)\n"
                    << "	addl 12, %esp\n"
                    << "    \n"
                    << "    movl 16(%ebp), %ecx # param @a (Type A)\n"
                    << "	\n"
                    << "    subl 4, %esp  # return value of getRow\n"
                    << "    pushl %ecx; pushl A_m_getRow; call (%ecx)\n"
                    << "	addl 8, %esp\n"
                    << "    popl %eax\n"
                    << "    \n"
                    << "    pushl %eax     # row\n"
                    << "    pushl %ecx; pushl A_m_test; call (%ecx)\n"
                    << "	addl 12, %esp\n"
                    << "	\n"
                    << "    popl %edx\n"
                    << "    popl %ecx\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
        // int getRow()
        {
            MethodDefNode &method = env().create<MethodDefNode>();
            method.name = "getRow";
            {
                InlinePasmInstructionNode &pasm = env().create<InlinePasmInstructionNode>();
                pasm.pasm
                    << "    movl 12(%ebp), %eax               // @this (Type B)\n"
                    << "    movl handle_B_vars_A(%eax), %ebx  // inst vars offset (A)\n"
                    << "    addl 4(%eax), %ebx                // @this.vars(A)\n"
                    << "    \n"
                    << "    movl A_i_row(%ebx), %eax     // row\n"
                    << "    addl %eax, %eax              // *2\n"
                    << "    movl %eax, 16(%ebp)          // return row*2\n"
                ;
                method.body.add(pasm);
            }
            cls.methods.add(method);
        }
    }
    return *bDef;
}
