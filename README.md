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

// You want to either do using for these, alias them or
// just pull the entire roc::import namespace, it's
// meant for that

// This is clearer
using roc::import::Some;
using roc::import::None;

// but just would do
// using namespace roc::import

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

Questions you were going to ask
-------------------------------

###  Why yet another either / optional type?

Mostly because I am suffering from bad case of NIH syndrome

Also, I want reference support for my optionals and expecteds, and I find both
assign-through and rebind semantics for those *extremely* bad.

Assign-through is one that makes sense in context of references, but it is not
consistent with itself.  (The assignment needs to do different things depending
on the state of the optional)

Rebind semantics do the same thing every time, but they have the problem that
they allow implicit rebinding, basically making the reference a pointer instead
in my eyes.

We handle that by allowing *initialisation* of reference optional/expected but
specifically delete the assignment operations for those.  If you want to do rebinds
or assign-throughs, you have to be explicit about them.


###  Why should I use this instead of [tartanllama/expected] or [tartanllama/optional]?

If you want a mature, well-tested, high-quality implementation you probably shouldn't.


###  Why is this not called `roc::optional` and `roc::expected`

Because I do not follow the standard implementations.


###  Are these audited for security and correctness?

*muffled laughter* No.  It would be cool to have that done though.


###  Do you actually expect somebody to ask any questions about this?

Not really, but I was bored so I wrote this.
