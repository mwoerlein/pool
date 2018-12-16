#include "linux/DirectoryPoolStorage.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "poolc/storage/Types.hpp"
#include "poolc/parser/token/FullQualifiedName.hpp"
#include "sys/stream/SeekableIOStream.hpp"

class StdFileIOStream2: public SeekableIOStream {
    private:
    std::FILE *file;
    size_t _offset;
    size_t _length;
    
    public:
    StdFileIOStream2(Environment &env, MemoryInfo &mi, std::FILE *file, size_t offset = 0)
        : Object(env, mi), file(file), _offset(offset) {
        std::fseek(file, 0, SEEK_END); // seek to end of file
        _length = std::ftell(file); // get current file pointer
        std::fseek(file, offset, SEEK_SET); // seek back to beginning
    }
    virtual ~StdFileIOStream2() { std::fclose(file); }
    
    using OStream::operator <<;
    virtual OStream &operator <<(char c) override {
        std::fputc(c, file);
        return *this;
    }
    using IStream::operator >>;
    virtual IStream & operator >>(char &c) override {
        c = std::fgetc(file);
        return *this;
    }
    virtual bool isEmpty() override {
        int last = std::fgetc(file);
        bool ret = last == EOF;
        std::ungetc(last, file);
        return ret;
    }
    virtual size_t length() override {
        return _length;
    }
    virtual size_t pos() override { return std::ftell(file) - _offset; }
    virtual void seek(size_t pos) override { std::fseek(file, pos + _offset, SEEK_SET); }
};

class StdFileElement: public StorageElement {
    private:
    friend class DirectoryPoolStorage;
    char filename[FILENAME_MAX];
    size_t _offset;
    
    public:
    StdFileElement(Environment &env, MemoryInfo &mi, String & filePath, size_t offset = -1)
        : StorageElement(env, mi), Object(env, mi), _offset(offset) {
        filePath >> filename;
    }
    virtual ~StdFileElement() {  }
    
    virtual IStream & getContent() {
        FILE * file = std::fopen(filename, "r+");
        if (!file) {
            return *(IStream*)0;
        }
        return env().create<StdFileIOStream2, std::FILE *, size_t>(file, (_offset == -1) ? 0 : _offset);
    }
};

// public
DirectoryPoolStorage::DirectoryPoolStorage(Environment & env, MemoryInfo & mi, const char * baseDir)
        :Object(env, mi), headerParser(env.create<HeaderParser>()) {
    this->baseDir = realpath(baseDir, 0);
}

DirectoryPoolStorage::~DirectoryPoolStorage() {
    headerParser.destroy();
}

StorageElement * DirectoryPoolStorage::getElement(String & classname, String & mimetype) {
    String & filePath = buildFilePath(classname, mimetype);
    StdFileElement &element = env().create<StdFileElement, String &>(filePath);
    filePath.destroy();
    
    if (IStream *input = &element.getContent()) {
        element._offset = headerParser.parse(*input, element);
        input->destroy();
    }
    
    if (element._offset < 0) {
        element.destroy();
        return 0;
    }
    return &element;
}

OStream & DirectoryPoolStorage::writeElement(String & classname, String & mimetype) {
    String & filePath = buildFilePath(classname, mimetype);
    char buffer[FILENAME_MAX];
    filePath >> buffer;
    filePath.destroy();
    std::FILE * file = std::fopen(buffer, "w+");
    if (!file) {
        env().err()<<"could not open file '"<<buffer<<"'\n";
        return *(OStream*) 0;
    }
    return env().create<StdFileIOStream2, std::FILE *>(file);
}

String & DirectoryPoolStorage::buildFilePath(String & classname, String & mimetype) {
    String & filePath = env().create<String, const char *>(baseDir);
    FullQualifiedName & fqn = env().create<FullQualifiedName>();
    fqn = classname;
    Iterator<String> & it = fqn.parts();
    while (it.hasNext()) {
        filePath << "/" << it.next();
    }
    if (mimetype == MIMETYPE_POOL) {
        filePath << ".pool";
    }
    return filePath;
}
