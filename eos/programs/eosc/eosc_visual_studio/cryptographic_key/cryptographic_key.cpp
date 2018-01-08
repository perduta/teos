// cryptographic_key.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <fc/crypto/sha256.hpp>

using namespace std;

//string key_to_wif(EC_KEY* k) {
//  k.get_secret();
//  return string();
//}

std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend)
{
  CAutoBN_CTX pctx;
  CBigNum bn58 = 58;
  CBigNum bn0 = 0;

  // Convert big endian data to little endian
  // Extra zero at the end make sure bignum will interpret as a positive number
  std::vector<unsigned char> vchTmp(pend - pbegin + 1, 0);
  reverse_copy(pbegin, pend, vchTmp.begin());

  // Convert little endian data to bignum
  CBigNum bn;
  bn.setvch(vchTmp);

  // Convert bignum to std::string
  std::string str;
  // Expected size increase from base58 conversion is approximately 137%
  // use 138% to be safe
  str.reserve((pend - pbegin) * 138 / 100 + 1);
  CBigNum dv;
  CBigNum rem;
  while (bn > bn0)
  {
    if (!BN_div(dv.to_bignum(), rem.to_bignum(), bn.to_bignum(), bn58.to_bignum(), pctx))
      throw bignum_error("EncodeBase58 : BN_div failed");
    bn = dv;
    unsigned int c = rem.getulong();
    str += pszBase58[c];
  }

  // Leading zeroes encoded as base58 zeros
  for (const unsigned char* p = pbegin; p < pend && *p == 0; p++)
    str += pszBase58[0];

  // Convert little endian std::string to big endian
  reverse(str.begin(), str.end());
  //    slog( "Encode '%s'", str.c_str() );
  return str;
}

std::string EncodeBase58(const vector<unsigned char>& vch)
{
  return EncodeBase58(&vch[0], &vch[0] + vch.size());
}
std::string to_base58(const char* d, size_t s) {
  return EncodeBase58((const unsigned char*)d, (const unsigned char*)d + s).c_str();
}

std::string key_to_wif(const fc::sha256& secret)
{
  const size_t size_of_data_to_hash = sizeof(secret) + 1;
  const size_t size_of_hash_bytes = 4;
  char data[size_of_data_to_hash + size_of_hash_bytes];
  data[0] = (char)0x80;
  memcpy(&data[1], (char*)&secret, sizeof(secret));
  fc::sha256 digest = fc::sha256::hash(data, size_of_data_to_hash);
  digest = fc::sha256::hash(digest);
  memcpy(data + size_of_data_to_hash, (char*)&digest, size_of_hash_bytes);
  return to_base58(data, sizeof(data));
}

int main()
{
  fc::sha256 key;
  EC_KEY* k = EC_KEY_new_by_curve_name(NID_secp256k1);
  if (!EC_KEY_generate_key(k)) {
    cerr << "ecc key generation error" << endl;
  }
  if (!k) {
    //key;
  }
  else {
    const BIGNUM* bn = EC_KEY_get0_private_key(k);
    if (bn == NULL) {
      cerr << "get private key failed" << endl;
    }
    int nbytes = BN_num_bytes(bn);
    fc::sha256 key;
    BN_bn2bin(bn, &((unsigned char*)&key)[32 - nbytes]);
  }
  EC_KEY_free(k);

  const size_t size_of_data_to_hash = sizeof(key) + 1;
  const size_t size_of_hash_bytes = 4;
  char data[size_of_data_to_hash + size_of_hash_bytes];
  data[0] = (char)0x80;
  memcpy(&data[1], (char*)&key, sizeof(key));
  fc::sha256 digest = fc::sha256::hash(data, size_of_data_to_hash);
  digest = fc::sha256::hash(digest);
  memcpy(data + size_of_data_to_hash, (char*)&digest, size_of_hash_bytes);


  cout << key << endl; 


  
  return 0;////mnt/hgfs/Workspaces/EOS/eos/libraries/fc/src/crypto/elliptic_common.cpp
