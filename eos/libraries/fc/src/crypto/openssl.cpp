#include <fc/crypto/openssl.hpp>

#include <fc/filesystem.hpp>

#include <boost/filesystem/path.hpp>

#include <cstdlib>
#include <string>
#include <stdlib.h>

#ifdef _MSC_VER
// secp256k1 needs it, if compiled with mingw, at least.
FILE __iob_func[3] = { *stdin,*stdout,*stderr };
#endif // _MSC_VER

namespace  fc 
{
    struct openssl_scope
    {
       static path _configurationFilePath;
       openssl_scope()
       {
          ERR_load_crypto_strings(); 
          OpenSSL_add_all_algorithms();

          const boost::filesystem::path& boostPath = _configurationFilePath;
          if(boostPath.empty() == false)
          {
            std::string varSetting("OPENSSL_CONF=");
            varSetting += _configurationFilePath.to_native_ansi_path();
#if defined(_MSC_VER)
            _putenv((char*)varSetting.c_str());
#else
            putenv((char*)varSetting.c_str());
#endif
          }

          OPENSSL_config(nullptr);
       }

       ~openssl_scope()
       {
          EVP_cleanup();
          ERR_free_strings();
       }
    };

    path openssl_scope::_configurationFilePath;

    void store_configuration_path(const path& filePath)
    {
      openssl_scope::_configurationFilePath = filePath;
    }
   
    int init_openssl()
    {
      static openssl_scope ossl;
      return 0;
    }
}
