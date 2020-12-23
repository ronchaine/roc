Result / Option for C++
=======================
Header only library for Result / Option.

By default, the headers can be compiled in freestanding environment,
and do not use exceptions.

In general, additional features are opt-in instead of opt-out, so
if you need extras, you need to use `#define` s or make your compiler
apply the preprocessor defines.

TODO:
-----
- [ ] Basic implementations
    - [x] option
    - [ ] result
    - [ ] monadic
- [ ] Tests
    - [ ] option
    - [ ] result
    - [ ] monadic

Quickstart
----------
Just copy `include/roc` to your include path.  You can also just pick
individual headers from there, but `result.hpp` and `option.hpp` depend
on `utility.hpp` (and on monadic branch, `monadic.hpp`)

Then write code like

```
#include <iostream>

#define ROC_ENABLE_STD_STREAMS

#include <roc/option.hpp>

// You want to either do using these, alias them or
// just pull the entire roc::import namespace, it's
// meant for that
using roc::import::Some;
using roc::import::None;

roc::option<int> test_function(int in) {
    if (in >= 0)
        return Some(-in);
    else
        return None;
}

int main()
{
    int a = 242;
    int b = 21;

    roc::option<int> maybe_int = Some(42);
    roc::option<int&> maybe_ref = Some(a);

    // rebinds are explicit
    maybe_ref.rebind(b);

    maybe_int = Some(52); // this can be reassigned

    // maybe_ref = Some(b);  // but we get compile-time error here

    a = 5432;
    b = 42;

    std::cout << "test_function(0): " << test_function(0) << "\n";
    std::cout << "test_function(8): " << test_function(8) << "\n";
    std::cout << "test_function(-5): " << test_function(-5) << "\n";

    std::cout << maybe_int << ", " << maybe_ref << " (a: " << a << ", b: " << b << ")\n";
}
```


Configuring
-----------
Compile time macro options:

Defining `ROC_ENABLE_STD_STREAMS` will create `std::ostream` specialisations
for the types, so they can be used with `std::cout` or similar tools.

Defining `ROC_ENABLE_EXCEPTIONS` will enable exception handling, and invalid
accesses are thrown as exceptions instead of panicing and calling abort
