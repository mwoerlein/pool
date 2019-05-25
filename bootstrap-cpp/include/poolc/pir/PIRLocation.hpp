#ifndef POOLC_PIR_PIRLOCATION_HPP_LOCK
#define POOLC_PIR_PIRLOCATION_HPP_LOCK

#include "sys/Object.hpp"

#include "poolc/ast/Type.hpp"

enum location_kind { loc_param, loc_ret, loc_spill, loc_temp, loc_this };
class PIRLocation: virtual public Object {
    public:
    location_kind kind;
    int idx;
    Type &type;
    PIRLocation(Environment &env, MemoryInfo &mi, Type &type, location_kind kind, int idx);
    virtual ~PIRLocation();
    
    OStream & operator >>(OStream & stream) override;
};

#endif //POOLC_PIR_PIRLOCATION_HPP_LOCK
