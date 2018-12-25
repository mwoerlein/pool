#include "linux/DirectoryPoolStorage.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "poolc/storage/Types.hpp"
#include "poolc/parser/token/FullQualifiedName.hpp"
#include "sys/stream/SeekableIOStream.hpp"

class StdFileIOStream2: public SeekableIOStream {
    private:
    std::FILE *file;
    size_t _offset;
    size_t _length;
    
    public:
    StdFileIOStream2(Environment &env, MemoryInfo &mi, std::FILE *file, size_t offset = 0, size_t length = -1)
        : Object(env, mi), file(file), _offset(offset) {
        if (length == -1) {
            std::fseek(file, 0, SEEK_END); // seek to end of file
            _length = std::ftell(file); // get current file pointer
            std::fseek(file, offset, SEEK_SET); // seek back to beginning
        } else {
            _length = _offset + length;
        }
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
        return std::ftell(file) >= _length;
    }
    virtual size_t length() override {
        return _length - _offset;
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
    
    virtual void dumpHeaders(OStream &out) {
        if (_offset == -1) {
            return;
        }
        FILE * file = std::fopen(filename, "r+");
        if (!file) {
            return;
        }
        IStream &in = env().create<StdFileIOStream2, std::FILE *, size_t, size_t>(file, 0, _offset);
        out << in;
        in.destroy();
    }
};

// public
DirectoryPoolStorage::DirectoryPoolStorage(Environment & env, MemoryInfo & mi, const char * baseDir)
        :Object(env, mi), headerParser(env.create<HeaderParser>()) {
    this->baseDir = realpath(baseDir, 0);
}

DirectoryPoolStorage::DirectoryPoolStorage(Environment & env, MemoryInfo & mi, String & baseDir)
        :Object(env, mi), headerParser(env.create<HeaderParser>()) {
    char buffer[FILENAME_MAX];
    baseDir >> buffer;
    this->baseDir = realpath(buffer, 0);
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
    
    if (element._offset == -1) {
        element.destroy();
        return 0;
    }
    return &element;
}

OStream & DirectoryPoolStorage::writeElement(String & classname, String & mimetype) {
    String & filePath = buildFilePath(classname, mimetype, true);
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

String & DirectoryPoolStorage::buildFilePath(String & classname, String & mimetype, bool createPath) {
    char buffer[FILENAME_MAX];
    String & filePath = env().create<String, const char *>(baseDir);
    FullQualifiedName & fqn = env().create<FullQualifiedName>();
    fqn = classname;
    Iterator<String> & it = fqn.parts();
    while (it.hasNext()) {
        if (createPath) {
            filePath >> buffer;
            mkdir(buffer, 0755);
        }
        filePath << "/" << it.next();
    }
    if (mimetype == MIMETYPE_POOL) {
        filePath << ".pool";
    }
    if (mimetype == MIMETYPE_PASM) {
        filePath << ".pasm";
    }
    return filePath;
}
