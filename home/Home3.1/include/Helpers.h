
#ifndef HELPERS_H_CPP2
#define HELPERS_H_CPP2


//=== Cpp2 type declarations ====================================================


#include "cpp2util.h"

#line 1 "Helpers.h2"


//=== Cpp2 type definitions and function declarations ===========================

#line 1 "Helpers.h2"
[[nodiscard]] auto dummyFunc(cpp2::impl::in<double> x) -> double;

#line 8 "Helpers.h2"
[[nodiscard]] auto dummyClassTask(cpp2::impl::in<std::vector<int>> vec, cpp2::impl::in<std::string> string_to_rep) -> std::string;

#line 27 "Helpers.h2"
[[nodiscard]] auto dummyHomeTask(cpp2::impl::in<double> x, cpp2::impl::in<double> y, cpp2::impl::in<double> z) -> double;

//=== Cpp2 function definitions =================================================

#line 1 "Helpers.h2"
[[nodiscard]] auto dummyFunc(cpp2::impl::in<double> x) -> double{
    // inspect as well as ternary disabled in cpp2

#line 4 "Helpers.h2"
    if ((cpp2::impl::cmp_greater(x,0))) {return x; }
    else {return 0; }
}

#line 8 "Helpers.h2"
[[nodiscard]] auto dummyClassTask(cpp2::impl::in<std::vector<int>> vec, cpp2::impl::in<std::string> string_to_rep) -> std::string{
    std::string output {}; 

    if ((CPP2_UFCS(empty)(vec))) {return output; }
    if ((CPP2_UFCS(empty)(string_to_rep))) {return output; }

    auto res {std::ranges::max_element(CPP2_UFCS(begin)(vec), CPP2_UFCS(end)(vec))}; 
    int iter {*cpp2::impl::assert_not_null(cpp2::move(res))}; 
{
auto i{0};

#line 18 "Helpers.h2"
    for( ; cpp2::impl::cmp_less(i,iter); 
    ++i ) 
    {
        output += string_to_rep;
    }
}

#line 24 "Helpers.h2"
    return output; 
}

#line 27 "Helpers.h2"
[[nodiscard]] auto dummyHomeTask(cpp2::impl::in<double> x, cpp2::impl::in<double> y, cpp2::impl::in<double> z) -> double{
    auto prod {x * y * z}; 
    if ((prod != 0)) {return 1 / cpp2::move(prod); }

    auto sum {x + y + z}; 
    if ((sum != 0)) {return 1 / cpp2::move(sum); }

    else {return x + (y + 1) * (z - 1); }
}
#endif

