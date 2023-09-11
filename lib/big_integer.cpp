#include "big_integer.h"


static const std::string err_symvol = "Число содержит недопустимый символ: ";
static const std::string err_null = "Число не определено.";

class MyException : public std::exception
{
    private:
    const std::string msg;

    public:
        MyException(const std::string msg_) : msg(msg_){}
        virtual ~MyException(){}

        const char* what() const noexcept override
        {
            return msg.c_str();
        }
};

bool big_integer::valid_digit(char symvol)
{
    if(symvol < '0' || symvol > '9')
    {
        const std::string err = err_symvol + std::to_string(symvol);
        throw MyException(err);
    }

    return true;
}

void big_integer::dig_init(const char* str)
{
    int i = 0;
    int null_count = 0;

    while(str[i] != 0)
    {
        i += static_cast<int>(valid_digit(str[i]));
    }

    int str_size = i + 1;

    i = 0;
    while(str[i] == '0' && i < str_size - 1)
    { 
        ++null_count;
        ++i;
    }

    size_ = str_size - null_count; 

    if (size_ <= 1)
    {
        str_ = new char[2]{'0', 0};
    }
    else if (size_ > 1)
    {
        str_ = new char[size_];
        str_[size_ - 1] = 0;

        for (int i = size_ - 2, j = str_size - 2; i >= 0; --i, --j)
        {
            str_[i] = str[j];
        }
    }
    else
    {
        throw MyException(err_null);
    }
}

big_integer::big_integer() : 
    size_{2},
    str_{new char[2]{'0', 0}}
{}

big_integer::big_integer(const char* str)
{
    dig_init(str);
}

big_integer::big_integer(const std::string& str)
{
    const char* chars = str.c_str();

    dig_init(chars);
}

big_integer::big_integer(const big_integer& other) 
:   str_(new char[other.size_]),
    size_(other.size_)
{
    std::copy(other.str_, other.str_ + other.size_, str_);
}

big_integer::big_integer(big_integer&& other) noexcept 
:  str_(std::exchange(other.str_, nullptr)),
   size_(std::exchange(other.size_, 0))
{}

big_integer& big_integer::operator=(const big_integer& other)
{
    if (this == &other)
        return *this;
    
    char* buff = str_;
    str_ = new char[other.size_];
    std::copy(other.str_, other.str_ + other.size_, str_);
    size_ = other.size_;
    delete[] buff;

    return *this;
}

big_integer& big_integer::operator=(big_integer&& other) noexcept 
{
    if (this == &other)
        return *this;

    delete[] str_;
    str_ = std::exchange(other.str_, nullptr);
    size_ = std::exchange(other.size_, 0);

    return *this;
}

big_integer& big_integer::operator=(const char* str)
{
    delete[] str_; 

    dig_init(str);

    return *this;
}

big_integer& big_integer::operator=(const std::string& str)
{
    delete[] str_;
    const char* chars = str.c_str();

    dig_init(chars);

    return *this;
}

const char* big_integer::to_str() 
{
    return str_;
}

big_integer big_integer::operator+(big_integer& other)
{
    big_integer* a;
    big_integer* b;
    char* c;
    int c_size = 0;

    if (size_ >= other.size_)
    {
        a = this;
        b = &other;
        c_size = size_ + 1;
    }
    else
    {
        a = &other;
        b = this;
        c_size = other.size_ + 1;
    }

//    auto c = chars_ptr(new char[c_size]);
    c = new char[c_size];
    c[0] = '0';

    int sum = 0;
    int r = 0;

    for (int i = b->size_ - 2, j = a->size_ - 2, k = c_size - 2;
        i >= 0;
        --i, --j, --k)
    {
        sum = (a->str_[j] - '0') + (b->str_[i] - '0') + r;
        r = sum / 10;
        c[k] = static_cast<char>(sum % 10 + '0');
        if (j >= 0 && i == 0)
        {
            --j;
            --k;
            if (r == 0)
            {
                for (; j >= 0; --j, --k)
                {
                    c[k] = a->str_[j];
                }
            }
            else
            {
                for(; r != 0 && j >= 0; --j, --k)
                {
                    sum = (a->str_[j] - '0') + r;
                    r = sum / 10;
                    c[k] = static_cast<char>(sum % 10 + '0');
                } 
            }
        }
    }

   // const char* chars = static_cast<const char*>(c);
    big_integer result(c);
    delete[] c;
    c = nullptr;

    return result;
}

big_integer big_integer::operator+=(big_integer&& other) noexcept
{
    big_integer sum = *this + other;
    
    *this = std::move(sum);

    return *this;
}

big_integer big_integer::operator*(big_integer& other)
{
    big_integer* col;
    big_integer* row;
    big_integer result;

    if (size_ > other.size_)
    {
        col = this;
        row = &other;
    }
    else
    {
        col = &other;
        row = this;
    }

    const unsigned int c_size = col->size_ + row->size_;

    int mult = 0;
    int r = 0;
    for (int i = row->size_ - 2, level = 0; i >= 0; --i, ++level)
    {
        char* c = new char[c_size];
        c[c_size - 1] = 0;
        int k = c_size - 2;
        if (level > 0)
        {
            for (int l = k; l >= k - level; --l)
            {
                c[l] = '0';
            }
            k -=level;
        }
        for (int j = col->size_ - 2; j >= 0; --j)
        {
            mult = (row->str_[i] - '0') * (col->str_[j] - '0') + r;
            r = mult / 10;
            c[k] = static_cast<char>(mult % 10 + '0');
            --k;
        }
        for (;k >= 0; --k)
        {
            c[k] = '0';
        }

        result += big_integer(c);
        delete[] c;
        c = nullptr;
    }
    
    return result;
}

// big_integer& big_integer::operator*(big_integer&& other) noexcept
// {
//     big_integer mult = *this * other;
//     return mult;
// }


std::ostream& operator<<(std::ostream& out, big_integer& obj)
{
    out << obj.to_str();

    return out;
}

big_integer::~big_integer()
{
    delete[] str_;
}

int big_integer::len()
{
    return size_ - 1;
}