#include <iostream>
#include <gtest/gtest.h>

#include "../../includes/fc4sc.hpp"

int main(int ac, char* av[])
{
  testing::InitGoogleTest(&ac,  av);
  
  return RUN_ALL_TESTS();
}


