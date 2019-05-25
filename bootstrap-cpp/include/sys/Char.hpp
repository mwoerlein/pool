#ifndef CHAR_HPP_LOCK
#define CHAR_HPP_LOCK

#include "sys/Comparable.hpp"
#include "sys/stream/OStream.hpp"

class Char: public ComparableC<char>, public Comparable<Char> {
    protected:
    char _c;
    
    public:
    Char(Environment &env, MemoryInfo &mi = *notAnInfo, char c = '\0');
    Char(Environment &env, MemoryInfo &mi, Char &c);
    virtual ~Char();
    
    operator char();
    virtual Char & operator =(char c);
    virtual Char & operator =(Char &c);
    
    useComparableCOperators(char);
    useComparableOperators(Char);
    virtual int compareTo(char c) override;
    virtual int compareTo(Char &c) override;
    virtual OStream & operator >>(OStream & stream) override;
    virtual int hash() override;
    
    // TODO: implement RTTI correctly
    virtual RTTI rtti() override;
};

#endif //CHAR_HPP_LOCK

