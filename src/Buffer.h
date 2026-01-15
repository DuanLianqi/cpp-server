#ifndef BUFFER_H
#define BUFFER_H

#include <string>

class Buffer {
public:
    Buffer();
    ~Buffer();

    void append(const char *_str, int size);
    ssize_t size();
    const char *c_str();
    void clear();
    void getline();
private:
    std::string buf;
};

#endif
