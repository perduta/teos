#pragma once

#include <stdlib.h>
#include <string>
#include <iostream>

using namespace std;

namespace tokenika{ 
  namespace eosc{

    class KeyPair {

    public:
      static string privateK();
      static string prk;
      string privateKey;
      string publicKey;

      KeyPair();
    };
  } 
}