#include "sys/String.hpp"

#include "sys/Digit.hpp"

class String::_Element: public Char {
    public:
    _Element *next;
    
    _Element(Environment &env, MemoryInfo &mi, char c): Char(env, mi, c), Object(env, mi), next(0) {}
    virtual ~_Element() {}
};

class String::_IStream: public IStream {
    private:
    _Element *ack;
    
    public:
    _IStream(Environment &env, MemoryInfo &mi, _Element &first): Object(env, mi), ack(&first) {}
    virtual ~_IStream() {}
    
    using IStream::operator >>;
    virtual IStream &operator >>(char &c) override {
        if (ack == 0) {
            c = '\0';
        } else {
            c = (char) *ack;
            ack = ack->next;
        }
    }
    virtual bool isEmpty() override {
        return ack == 0;
    }
};

// public
String::String(Environment &env, MemoryInfo &mi): Object(env, mi), first(0), last(0), _length(0), _hash(0) {}
String::String(Environment &env, MemoryInfo &mi, String &s): Object(env, mi), first(0), last(0), _length(0), _hash(0) {
    *this << s;
}
String::String(Environment &env, MemoryInfo &mi, char *cstring): Object(env, mi), first(0), last(0), _length(0), _hash(0) {
    *this << cstring;
}
String::String(Environment &env, MemoryInfo &mi, const char *cstring): Object(env, mi), first(0), last(0), _length(0), _hash(0) {
    *this << cstring;
}
String::String(Environment &env, MemoryInfo &mi, char *start, char* end): Object(env, mi), first(0), last(0), _length(0), _hash(0) {
    for (char *cur = start; cur < end; cur++) {
        *this << *cur;
    }
}
String::~String() {
    clear();
}

String & String::operator =(char *cstring) {
    clear();
    *this << cstring;
    return *this;
}

String & String::operator =(const char *cstring) {
    clear();
    *this << cstring;
    return *this;
}

String & String::operator =(String &s) {
    clear();
    *this << s;
    return *this;
}

OStream & String::operator <<(char c) {
    _Element *newElem = &env().create<_Element, char>(c);
    if (last) {
        last = last->next = newElem;
    } else {
        first = last = newElem;
    }
    _length++;
    _hash += _hash << 5;
    _hash += c;
    return *this;
}

int String::compareTo(char *cstring) {
    _Element *e1 = first;
    char *e2 = cstring;
    while (e1 && e2) {
        int diff = (char) *e1 - *e2;
        if (diff) {
            return diff;
        }
        e1 = e1->next;
        e2++;
    }
    if (e1) {
        return (int) (char) *e1;
    }
    if (e2) {
        return -(int) *e2;
    }
    return 0;
}

int String::compareTo(const char *cstring) {
    return compareTo((char*) cstring);
}

int String::compareTo(String &s) {
    _Element *e1 = first;
    _Element *e2 = s.first;
    while (e1 && e2) {
        int diff = (char) *e1 - (char) *e2;
        if (diff) {
            return diff;
        }
        e1 = e1->next;
        e2 = e2->next;
    }
    if (e1) {
        return (int) (char) *e1;
    }
    if (e2) {
        return -(int) (char) *e2;
    }
    return 0;
}

IStream & String::toIStream() {
    return env().create<_IStream, _Element&>(*first);
};

int String::length() {
    return _length;
}

int String::hash() {
    return _hash;
}

RTTI String::rtti() {
    return string;
}

OStream & String::operator >>(OStream & stream) {
    for (_Element * cur = first; cur; cur = cur->next) {
        stream << *cur;
    }
    return stream;
}

void String::escapeToStream(OStream &stream, char enclosure, char escape) {
    stream << enclosure;
    for (_Element * cur = first; cur; cur = cur->next) {
        char c = (char) *cur;
        if ((c == enclosure) || (c == escape)) {
            stream << escape << *cur;
        } else if (c == '\n') {
            stream << escape << 'n';
        } else if (c == '\r') {
            stream << escape << 'r';
        } else if (c == '\t') {
            stream << escape << 't';
        } else {
            stream << *cur;
        }
    }
    stream << enclosure;
}

void String::operator >>(char *buffer) {
    for (_Element * cur = first; cur; cur = cur->next) {
        *buffer++ = (char) *cur;
    }
    *buffer = 0;
}

void String::operator >>(int & i) {
    i = 0;
    if (!first) {
        return; // zero
    }
    int sign = 1;
    int base = 10;
    
    _Element *cur = first;
    if (*cur == '-') {
        sign = -1;
        cur = cur->next;
    }
    if (*cur == '0') {
        if (!cur->next) {
            return; // zero
        }
        switch ((char) *cur->next) {
            case 'b':
            case 'B':
                base = 2;
                cur = cur->next->next;
                break;
            case 'x':
            case 'X':
                base = 16;
                cur = cur->next->next;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                base = 8;
                cur = cur->next;
                break;
            default:
                return; // zero
        }
    }
    
    int result = 0;
    Digit d(env());
    for (; cur; cur = cur->next) {
        d = (char) *cur;
        if ((int) d < base) {
            result *= base;
            result += (int) d;
        } else {
            // end of number
            break;
        }
    }
    i = sign * result;
}

// protected

void String::clear() {
    _Element * cur = first;
    while (cur) {
        _Element * kill = cur;
        cur = cur->next;
        kill->destroy();
    }
    last = first = 0;
    _length = _hash = 0;
}
