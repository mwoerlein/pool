#include "poolc/pir/PIRLocation.hpp"

// public
PIRLocation::PIRLocation(Environment &env, MemoryInfo &mi, Type &type, location_kind kind, int idx)
        :Object(env, mi), type(type), kind(kind), idx(idx) {}
PIRLocation::~PIRLocation() {}

OStream & PIRLocation::operator >>(OStream & stream) {
    switch (kind) {
        case loc_param:
            return stream << "<" << type << ">param("<<idx<<")";
        case loc_ret:
            return stream << "<" << type << ">ret("<<idx<<")";
        case loc_spill:
            return stream << "<" << type << ">spill("<<idx<<")";
        case loc_temp:
            return stream << "<" << type << ">temp("<<idx<<")";
        case loc_this:
            return stream << "<" << type << ">this";
    }
    return stream;
}
