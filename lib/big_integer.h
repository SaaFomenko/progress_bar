#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <memory>
#include <iostream>

//using chars_ptr = std::unique_ptr<char[]>;

class big_integer
{
    private:
        int size_;
        char* str_;
    
    public:
        big_integer();
        big_integer(const char* str);
        big_integer(const std::string& str);

        big_integer(const big_integer& other);
        big_integer(big_integer&& other) noexcept;

        big_integer& operator=(const big_integer& other);
        big_integer& operator=(big_integer&& other) noexcept;
        big_integer& operator=(const char* str);
        big_integer& operator=(const std::string& str);

        virtual ~big_integer();

        big_integer operator+(big_integer& other);
        big_integer operator+=(big_integer&& other) noexcept;

        big_integer operator*(big_integer& other);

        friend std::ostream& operator<<(std::ostream& out, big_integer& obj);

        bool valid_digit(char symvol);
        const char* to_str();
        void dig_init(const char* str);
        int len();
};

#endif