#include <UnitTests.hpp>
#include "MatchingEngine.h"
#include "ExchangeCore.h"
#include "Client.h"

auto main() -> int
{
    UnitTests testSuite;

    testSuite.addTest("P0 Matching test 1",
    [](){
        // Build:
        MatchingEngine engine("USDUAH");
    
        // Operate:
        auto test = engine.get_instrument();

        // Check:
        ASSERT_EQ(test, "USDUAH")
    });

    testSuite.addTest("P0 Matching test 2",
    [](){
        // Build:
        MatchingEngine engine("USDUAH");
    
        // Operate:
        auto test = engine.print_LOB();

        // Check:
        ASSERT_EQ(test.size(), 856)
    });

    testSuite.addTest("P0 ExchangeCore test 1",
    [](){
        // Build:
        ExchangeCore exch;
    
        // Operate:
        exch.debug_init_matchings({"USDUAH"});
        auto test = exch.debug_get_matching_inst();

        // Check:
        ASSERT_EQ(test, "USDUAH")
    });

    testSuite.run();
}
