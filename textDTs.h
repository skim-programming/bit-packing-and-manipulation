#include <iostream>
#include <cstdint>
#include <stdio.h>
#include <cstring>

struct custr{
    private:
    uint32_t len;
    char* data;
    
    static uint32_t strlen(const char* start) {
        const char *end = start;
        while(*end != '\0'){
            end++;
        }
        return end - start;
    }
    
    public:
    const char* getData() const {return data;};
    
    uint32_t size() const {return len;};
    
    void reset() {
        len = 0;
        delete[] data;
        data = new char[len+1];
        data[0] = '\0';
    }
    
    custr() : len(0), data(new char[len+1]) {data[0] = '\0';};
    explicit custr(const char* str) : len(strlen(str)), data(new char[len+1]){
        std::strcpy(data, str);
        std::cout << "Allocated memory\n";
    }
    ~custr(){
        delete[] data;
        std::cout << "Deallocated memory\n";
    }
    custr(const custr& cpy) : len(cpy.len), data(new char[len+1]) {
        std::strcpy(data, cpy.data);
        std::cout << "Created a copy\n";
    }
    custr(custr&& cpy) noexcept
    : len(cpy.len), data(cpy.data)
    {
        std::cout << "Move constructor called\n";
        cpy.len = 0;
        cpy.data = nullptr;
    }
    custr& operator=(const char* str) {
        if(!str){
            return *this;
        }
        delete[] data;
        
        len = strlen(str); 
        data = new char[len+1];
        std::strcpy(data, str);
        return *this;
        }
    custr& operator=(const custr& cpy){
        std::cout << "Copy constructor called\n";
        if (this == &cpy) return *this;
        
        delete[] data;
    
        len = cpy.len;
        data = new char[len+1];
        std::strcpy(data, cpy.data);
        return *this;
    }
    custr& operator=(custr&& cpy) noexcept {
        std::cout << "Move copy constructor called\n";
        if (this == &cpy) return *this;
        delete[] data;
        len = cpy.len;
        data = cpy.data;
        
        cpy.len = 0;
        cpy.data = nullptr;
        return *this;
    }
    void print() const {
        if(data) std::cout << data;
        std::cout << "\n";
    }
    
    friend std::ostream& operator<<(std::ostream& os, custr const& str){
        if(str.data) os << str.data;
        return os;
    }
};
