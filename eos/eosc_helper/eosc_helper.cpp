#include <eos/types/types.hpp>
#include <eos/types/public_key.hpp>
#include <fc/crypto/base58.hpp>

#include <fc/io/raw.hpp>
#include <fc/crypto/hmac.hpp>
#include <fc/crypto/openssl.hpp>
#include <fc/crypto/ripemd160.hpp>

#include <eos/utilities/key_conversion.hpp>
#include <fc/crypto/base58.hpp>
#include <fc/variant.hpp>

#include <eosc_helper.hpp>

using namespace std;

namespace tokenika {
  namespace eosc {

    KeyPair::KeyPair() {
      fc::ecc::private_key pk = fc::ecc::private_key::generate();
      publicKey = string(eosio::types::public_key(pk.get_public_key()));
      privateKey = eosio::utilities::key_to_wif(pk.get_secret());
    }
  }
}