#ifndef BUFFER_H
#define BUFFER_H

#include <string>

class Buffer {
public:
    Buffer();
    ~Buffer();

    void append(const char *_str, int size);
    size_t size();
    const char *c_str();
    void clear();
    void getline();
    void setBuf(const char* str);
private:
    std::string buf;
};

#endif
