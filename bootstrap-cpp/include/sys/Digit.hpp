#ifndef DIGIT_HPP_LOCK
#define DIGIT_HPP_LOCK

#include "sys/Char.hpp"
#include "sys/Integer.hpp"

class Digit: public Char {
    private:
    
    inline char itod(int d) {
        return (d < 0 || d > 36) ? '?' : ((d < 10) ? ('0' + d) : ('a' - 10 + d));
    }
    inline char ctod(char c) {
        if ('0' <= c && c <= '9') { return c; }
        if ('a' <= c && c <= 'z') { return c; } 
        if ('A' <= c && c <= 'Z') { return c - 'A' + 'a'; } 
        return '?';
    }
    inline int ctoi(char c) {
        if ('0' <= c && c <= '9') { return c - '0'; }
        if ('a' <= c && c <= 'z') { return c - 'a' + 10; } 
        if ('A' <= c && c <= 'Z') { return c - 'A' + 10; } 
        return -1;
    }
    
    public:
    Digit(Environment &env, MemoryInfo &mi = *notAnInfo, char c = '0');
    Digit(Environment &env, MemoryInfo &mi, Char &d);
    Digit(Environment &env, MemoryInfo &mi, int i);
    Digit(Environment &env, MemoryInfo &mi, Integer &i);
    virtual ~Digit();
    
    operator int();
    virtual Digit & operator =(char c) override;
    virtual Digit & operator =(Char &c) override;
    virtual Digit & operator =(Digit &c);
    virtual Digit & operator =(int i);
    virtual Digit & operator =(Integer &i);
};

#endif //DIGIT_HPP_LOCK

