#include "sys/collection/PropertyContainer.hpp"

// public
PropertyContainer::PropertyContainer(Environment &env, MemoryInfo &mi):
        HashMap(env, mi), LinkedList(env, mi), Object(env, mi) {
}

PropertyContainer::~PropertyContainer() {
    Iterator<Object> &ownedIterator = LinkedList::iterator();
    while (ownedIterator.hasNext()) {
        ownedIterator.next().destroy();
    }
    ownedIterator.destroy();
}

bool PropertyContainer::hasProperty(const char* name) {
    String &tmp = env().create<String, const char*>(name); 
    bool ret = hasProperty(tmp);
    tmp.destroy();
    return ret;
}
    
Object & PropertyContainer::getProperty(const char* name) {
    String &tmp = env().create<String, const char*>(name); 
    Object &ret = getProperty(tmp);
    tmp.destroy();
    return ret;
}
    
bool PropertyContainer::hasStringProperty(const char* name) {
    String &tmp = env().create<String, const char*>(name); 
    bool ret = hasStringProperty(tmp);
    tmp.destroy();
    return ret;
}
    
String & PropertyContainer::getStringProperty(const char* name) {
    String &tmp = env().create<String, const char*>(name); 
    String &ret = getStringProperty(tmp);
    tmp.destroy();
    return ret;
}

void PropertyContainer::dumpProperties(OStream &stream) {
    Iterator<String> &names = keys();
    while (names.hasNext()) {
        String & name = names.next();
        stream << name << "=" << getProperty(name) << '\n';
    }
    names.destroy();
}
