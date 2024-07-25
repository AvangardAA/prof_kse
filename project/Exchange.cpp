#include "ExchangeCore.h"

auto main() -> int 
{
  try
  {
      if (sodium_init() < 0) { throw; }

      ExchangeCore exchange;

      exchange.run();
  }
  catch (std::exception& e)
  {
      std::println("Critical error occured: exchange cant run further.");
  }

  return 0;
}