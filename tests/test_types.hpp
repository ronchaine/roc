#ifndef ROC_TEST_TYPES_HPP
#define ROC_TEST_TYPES_HPP

struct trivial_type {
    trivial_type() = default;
    trivial_type(const trivial_type&) = default;
    trivial_type(trivial_type&&) = default;
    trivial_type& operator=(const trivial_type&) = default;
    trivial_type& operator=(trivial_type&&) = default;
    ~trivial_type() = default;
};
            
struct nontrivial_noexcept_type {
    nontrivial_noexcept_type() noexcept {};
    nontrivial_noexcept_type(const nontrivial_noexcept_type&) noexcept {};
    nontrivial_noexcept_type(nontrivial_noexcept_type&&) noexcept {};
    nontrivial_noexcept_type& operator=(const nontrivial_noexcept_type&) noexcept { return *this; };
    nontrivial_noexcept_type& operator=(nontrivial_noexcept_type&&) noexcept { return *this; }
    ~nontrivial_noexcept_type() {};
};

struct nontrivial_type {
    nontrivial_type() {};
    nontrivial_type(const nontrivial_type&) {};
    nontrivial_type(nontrivial_type&&) {};
    nontrivial_type& operator=(const nontrivial_type&) { return *this; };
    nontrivial_type& operator=(nontrivial_type&&) { return *this; }
    ~nontrivial_type() {};
};

#endif
