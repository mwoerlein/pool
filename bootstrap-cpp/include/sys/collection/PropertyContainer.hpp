#ifndef SYS_COLLECTION_PROPERTYCONTAINER_HPP_LOCK
#define SYS_COLLECTION_PROPERTYCONTAINER_HPP_LOCK

#include "sys/Object.hpp"
#include "sys/String.hpp"
#include "sys/collection/HashMap.hpp"
#include "sys/collection/LinkedList.hpp"

class PropertyContainer: protected HashMap<String, Object>, protected LinkedList<Object>, virtual public Object {
    private:
    inline int size() { return HashMap::size(); }
    inline bool isEmpty() { return HashMap::isEmpty(); }
    inline bool contains(Object & o) { return HashMap::contains(o); }
    inline Iterator<String> & properties() { return HashMap::keys(); }
    inline Iterator<Object> & iterator() { return HashMap::iterator(); }
    
    public:
    PropertyContainer(Environment &env, MemoryInfo &mi = *notAnInfo);
    virtual ~PropertyContainer();
    
    // TODO: implement ownership correctly
    template <class Obj, typename... Args> inline Obj & createOwn(Args... args) {
        return transferOwnership(env().create<Obj, Args...>(args...));
    }
    template<class Obj> inline Obj & transferOwnership(Obj &o) {
        LinkedList::add(o);
        return o;
    }
    
    // Object accessors
    virtual bool hasProperty(const char* name);
    inline bool hasProperty(String & name) {
        return HashMap::has(name);
    }
    virtual Object & getProperty(const char* name);
    inline Object & getProperty(String & name) {
        return HashMap::get(name);
    }
    inline Object & setProperty(const char* name, Object & o) {
        return HashMap::set(createOwn<String, const char*>(name), o);
    }
    inline Object & setProperty(String & name, Object & o) {
        return HashMap::set(name, o);
    }
    inline Object & unsetProperty(const char* name) {
        return HashMap::unset(createOwn<String, const char*>(name));
    }
    inline Object & unsetProperty(String & name) {
        return HashMap::unset(name);
    }
    
    // String accessors
    virtual bool hasStringProperty(const char* name);
    virtual String & getStringProperty(const char* name); 
    
    inline bool hasStringProperty(String & name) {
        return env().as<String>(HashMap::get(name), string);
    }
    inline String & getStringProperty(String & name) {
        return *env().as<String>(HashMap::get(name), string);
    }
    inline Object & setStringProperty(const char* name, const char* value) {
        return HashMap::set(createOwn<String, const char*>(name), createOwn<String, const char*>(value));
    }
    inline Object & setStringProperty(const char* name, String & value){
        return HashMap::set(createOwn<String, const char*>(name), value);
    }
    inline Object & setStringProperty(String & name, const char* value){
        return HashMap::set(name, createOwn<String, const char*>(value));
    }
    inline Object & setStringProperty(String & name, String & value){
        return HashMap::set(name, value);
    }

    virtual bool testStringProperty(const char* name, const char* value) {
        return hasStringProperty(name) && (getStringProperty(name) == value);
    }
    
    // TODO: add Integer/Bool/... property accessors and conversions

    virtual void dumpProperties(OStream &stream);
};

#endif //SYS_COLLECTION_PROPERTYCONTAINER_HPP_LOCK
