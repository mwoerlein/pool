#include "pasm/i386/ASMInstructionList.hpp"

#include "sys/stream/NullIOStream.hpp"
#include "pasm/i386/Operand/Number.hpp"

class ASMInstructionList::_Elem: public ASMContext {
    public:
    _Elem * next;
    ASMInstruction *inst;
    String *identifier;
    Numeric *value;
    bool global;
    
    _Elem(Environment &env, MemoryInfo &mi, BitWidth mode)
        :ASMContext(env, mi, mode, mode, mode), Object(env, mi), inst(0), identifier(0), value(0), next(0), global(false) {}
    _Elem(Environment &env, MemoryInfo &mi, String *identifier, Numeric *value, BitWidth mode)
        :ASMContext(env, mi, mode, mode, mode), Object(env, mi), inst(0), identifier(identifier), value(value), next(0), global(false) {}
    _Elem(Environment &env, MemoryInfo &mi, ASMInstruction *inst, BitWidth mode, BitWidth data, BitWidth addr)
        :ASMContext(env, mi, mode, data, addr), Object(env, mi), inst(inst), identifier(0), value(0), next(0), global(false) {}
    virtual ~_Elem() {
        if (inst) {
            inst->destroy();
        }
        if (identifier) {
            identifier->destroy();
        }
        if (value) {
            value->destroy();
        }
    }
};

// public
ASMInstructionList::ASMInstructionList(Environment &env, MemoryInfo &mi, OStream &error, bool silent)
        :Object(env, mi),
         warn(silent ? (OStream&) env.create<NullIOStream>() : (OStream&) env.create<ParseErrorStream, OStream&>(error)),
         err(env.create<ParseErrorStream, OStream&>(error)),
         ids(env.create<HashMap<String, _Elem>>()), pos(-1), first(0), last(0) {
    setMode(bit_32);
}
ASMInstructionList::~ASMInstructionList() {
    _Elem * cur = first;
    while (cur) {
        _Elem * kill = cur;
        cur = cur->next;
        kill->destroy();
    }
    last = first = 0;
    pos = -1;
    ids.destroy();
    warn.destroy();
    err.destroy();
}

void ASMInstructionList::setMode(BitWidth mode) {
    if (last && !last->inst && !last->identifier) {
        // reuse previous mode, if available
        last->mode = mode;
        return;
    }
    _Elem * e = &env().create<_Elem, BitWidth>(mode);
    if (!first) {
        first = last = e;
    } else {
        last = last->next = e;
    }
}

void ASMInstructionList::addInstruction(ASMInstruction &inst, BitWidth data, BitWidth addr) {
    _Elem * e = &env().create<_Elem, ASMInstruction*, BitWidth, BitWidth, BitWidth>(
        &inst,
        last->mode,
        (data == bit_auto) ? last->mode : data,
        (addr == bit_auto) ? last->mode : addr
    );
    last = last->next = e;
    inst.list = this;
    inst.ctx = e;
}

void ASMInstructionList::addLabel(String &label) {
    if (ids.has(label)) {
        err << "Duplicate definition of id '" << label << "'\n";
        return;
    }
    _Elem * e = &env().create<_Elem, String*, Numeric*, BitWidth>(&label, 0, last->mode);
    last = last->next = e;
    ids.set(label, *e);
}

void ASMInstructionList::addDefinition(String &definition, Numeric &value, bool global) {
    if (ids.has(definition)) {
        err << "Duplicate definition of id '" << definition << "'\n";
        return;
    }
    _Elem * e = &env().create<_Elem, String*, Numeric*, BitWidth>(&definition, &value, last->mode);
    e->global = global;
    last = last->next = e;
    ids.set(definition, *e);
}

bool ASMInstructionList::hasLabel(String &label) {
    return ids.has(label) && !ids.get(label).value;
}

bool ASMInstructionList::hasDefinition(String &label) {
    return ids.has(label) && ids.get(label).value;
}

bool ASMInstructionList::isConstantDefinition(String &label) {
    return ids.has(label) && ids.get(label).value && ids.get(label).value->isConstant(*this);
}

size_t ASMInstructionList::getLabel(String &label) {
    return ids.get(label).pos;
}

int ASMInstructionList::getValue(String &label) {
    _Elem & e = ids.get(label);
    return e.value ? e.value->getValue(*this) : e.pos;
}

Numeric & ASMInstructionList::getNumeric(String &label) {
    _Elem & e = ids.get(label);
    return e.value ? e.value->clone() : env().create<Number,int>(e.pos);
}

size_t ASMInstructionList::compile() {
    if (pos != -1) {
        err << "List is already compiled!\n";
        return pos;
    }
    pos = 0;
    for (_Elem * cur = first; cur ; cur = cur->next) {
        cur->prePos = cur->pos = pos;
        if (cur->inst) { pos += cur->inst->prepare(); }
    }
    pos = 0;
    for (_Elem * cur = first; cur ; cur = cur->next) {
        cur->pos = pos;
        if (cur->inst) { pos += cur->inst->compile(); }
    }
    return pos;
}

void ASMInstructionList::finalize(size_t startAddress) {
    if (pos == -1) {
        err << "List is not compiled!\n";
        return;
    }
    if (startAddress) {
        for (_Elem * cur = first; cur ; cur = cur->next) {
            cur->pos += startAddress;
        }
    }
}

bool ASMInstructionList::hasErrors() {
    return (int) err;
}

void ASMInstructionList::writeToStream(OStream &stream) {
    if (pos == -1) {
        err << "List is not compiled!\n";
        return;
    }
    for (_Elem * cur = first; cur ; cur = cur->next) {
        if (cur->inst) {
            cur->inst->writeToStream(stream);
        }
    }
}

class DebugOStreamWrapper: public OStream {
    private:
    OStream &out;
    
    public:
    using OStream::operator <<;
    DebugOStreamWrapper(Environment &env, MemoryInfo &mi, OStream &out): Object(env, mi), out(out) {}
    virtual ~DebugOStreamWrapper() { }
    
    virtual OStream &operator <<(char c) override {
        (out<<' ').printuhex((unsigned char)c,2);
        return *this;
    }
};

void ASMInstructionList::logToStream(OStream &stream, bool debug) {
    for (_Elem * cur = first; cur ; cur = cur->next) {
        if (cur->inst) {      
            if (cur->data != cur->mode) switch (cur->data) {
                case bit_32: stream << ".data32 "; break;
                case bit_16: stream << ".data16 "; break;
            }
            if (cur->addr != cur->mode) switch (cur->addr) {
                case bit_32: stream << ".addr32 "; break;
                case bit_16: stream << ".addr16 "; break;
            }
            stream << *cur->inst;
            if (debug) {
                DebugOStreamWrapper wrap(env(), *notAnInfo, stream);
                stream << "\t// " << (void*) cur->pos << ": (" << cur->inst->size << ")";
                cur->inst->writeToStream(wrap);
            }
        } else if (cur->identifier) {
            if (cur->value) {
                if (pos != -1 && cur->value->isConstant(*this)) {
                    // skip constant definitions after compilation
                    // instructions replace definition usages with corresponding numbers
                    if (debug) {
                        stream << "// ";
                    } else {
                        continue;
                    }
                } 
                stream << *cur->identifier << " := " << *cur->value;
            } else {
                stream << *cur->identifier << ":";
            }
        } else {
            switch (cur->mode) {
                case bit_32: stream << ".code32"; break;
                case bit_16: stream << ".code16"; break;
            }
        }
        stream << '\n';
    }
}

void ASMInstructionList::logGlobalsToStream(OStream &stream) {
    if (pos == -1) {
        err << "List is not compiled!\n";
        return;
    }
    for (_Elem * cur = first; cur ; cur = cur->next) {
        if (cur->global && cur->identifier && cur->value) {
            (stream << *cur->identifier << " := ").printuhex(cur->value->getValue(*this)) << '\n';
        }
    }
}
