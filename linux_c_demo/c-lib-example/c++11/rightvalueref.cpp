#include <iostream>
#include <utility> // for std::move
#include <string.h>

class MyString
{
private:
    char *data;

public:
    // Constructor
    MyString(const char *s)
    {
        std::cout << "Regular constructor called" << std::endl;
        size_t length = strlen(s) + 1;
        data = new char[length];
        strcpy(data, s);
    }

    // Move constructor (takes a right-value reference)
    MyString(MyString &&other) noexcept
    {
        std::cout << "Move constructor called" << std::endl;
        data = other.data;
        other.data = nullptr; // Transfer ownership
    }

    // Move assignment operator
    MyString &operator=(MyString &&other) noexcept
    {
        std::cout << "Move assignment operator called" << std::endl;
        if (this != &other)
        {
            delete[] data; // Release current resources
            data = other.data;
            other.data = nullptr; // Transfer ownership
        }
        return *this;
    }

    // Destructor
    ~MyString()
    {
        std::cout << "Destructor called" << std::endl;
        delete[] data;
    }

    // Other member functions...

    // Display the string
    void display() const
    {
        std::cout << "String: " << data << std::endl;
    }
};

int main()
{
    // Create a MyString object using the regular constructor
    MyString regularStr("Hello, World!");
    regularStr.display();

    // Create a MyString object using the move constructor (right-value reference)
    MyString &&rvRefStr = MyString("C++11");
    rvRefStr.display();

    // Use std::move to cast an l-value to an r-value reference
    MyString anotherStr = std::move(regularStr);
    anotherStr.display();

    return 0;
}
