#ifndef INTEGER_HPP_LOCK
#define INTEGER_HPP_LOCK

#include "sys/Comparable.hpp"

class Integer: public ComparableC<int>, public Comparable<Integer> {
    private:
    int _i;
    
    public:
    Integer(Environment &env, MemoryInfo &mi = *notAnInfo, int i = 0);
    Integer(Environment &env, MemoryInfo &mi, Integer &i);
    virtual ~Integer();
    
    operator int();
    virtual Integer & operator =(int i);
    virtual Integer & operator =(Integer &i);

    virtual Integer & operator ++();
    virtual Integer & operator --();
    
    virtual Integer & operator +=(int i);
    virtual Integer & operator -=(int i);
    virtual Integer & operator *=(int i);
    virtual Integer & operator /=(int i);
    virtual Integer & operator %=(int i);
    
    virtual Integer & operator +=(Integer &i);
    virtual Integer & operator -=(Integer &i);
    virtual Integer & operator *=(Integer &i);
    virtual Integer & operator /=(Integer &i);
    virtual Integer & operator %=(Integer &i);
    
    useComparableCOperators(int);
    useComparableOperators(Integer);
    virtual int compareTo(int i) override;
    virtual int compareTo(Integer &i) override;
    virtual OStream & operator >>(OStream & stream) override;
    virtual int hash() override;
        
    // TODO: implement RTTI correctly
    virtual RTTI rtti() override;
};

#endif //INTEGER_HPP_LOCK

