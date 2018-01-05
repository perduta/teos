#include <stdlib.h>
#include <string>
#include <iostream>

#include <eosc_helper.hpp>

using namespace std;

int main()
{
  tokenika::eosc::KeyPair keyPair;
  cout << keyPair.privateKey << endl;
  cout << keyPair.publicKey << endl;
}

