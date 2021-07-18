#ifndef MEMORY_H
#define MEMORY_H

template<typename T>
class Safeptr {
    T* ptr;
public:
    friend T* operator=(T*& arg);
    T* get() const;
    Safeptr();
    Safeptr(T* ptr);

    ~Safeptr();
};

#endif // MEMORY_H
