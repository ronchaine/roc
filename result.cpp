#include <roc/result.hpp>

#include <iostream>
#include <vector>

using namespace roc::import;

int main(int argc, char* argv[])
{
    {
        std::cout << "init (21)\n";

        roc::result<int, int> int_test = Ok(21);
        std::cout << std::boolalpha << "int_test is value: " << int_test.is_ok() << "\n";
        std::cout << std::boolalpha << "int_test is error: " << int_test.is_err() << "\n";
    }
    {
        std::cout << "init (int = 20)\n";
        int a = 20;

        roc::result<int, int> int_test = Ok(static_cast<int>(a));
        std::cout << std::boolalpha << "int_test is value: " << int_test.is_ok() << "\n";
        std::cout << std::boolalpha << "int_test is error: " << int_test.is_err() << "\n";
    }
    {
        std::cout << "init (ref to int = 22)\n";
        int value = 22;

        roc::result<int&, int> intref_test = Ok(value);
        std::cout << std::boolalpha << "intref_test is value: " << intref_test.is_ok() << "\n";
        std::cout << std::boolalpha << "intref_test is error: " << intref_test.is_err() << "\n";

        std::cout << "address of value: " << (intptr_t)value << "\n";
        std::cout << "address of ref: " << (intptr_t)&intref_test.unwrap() << "\n";
    }

    return 0;
}
