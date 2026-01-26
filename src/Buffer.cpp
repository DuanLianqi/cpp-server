#include <iostream>
#include "Buffer.h"

Buffer::Buffer() {

}

Buffer::~Buffer() {

}

void Buffer::append(const char *_str, int size) {
    for(int i = 0; i < size; i++) {
        if(_str[i] == '\0') {
            break;
        }
        buf.push_back(_str[i]);
    }
}

size_t Buffer::size() {
    return buf.size();
}

const char* Buffer::c_str() {
    return buf.c_str();
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::getline() {
    buf.clear();
    std::getline(std::cin, buf);
}

void Buffer::setBuf(const char* str) {
    buf.clear();
    buf.append(str);
}
