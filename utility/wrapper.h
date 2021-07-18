#ifndef WRAPPER_H
#define WRAPPER_H

class wrapper {
private:
    void* memory;
public:
    virtual void get() const = 0;
    virtual void set(void mem) = 0;

};

#endif // WRAPPER_H
