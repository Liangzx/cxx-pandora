find_package(OpenSSL REQUIRED)
#add_executable(base64_demo base64.cpp)
#target_link_libraries(base64_demo OpenSSL::Crypto)
include_directories(/usr/local/include/openssl)
add_executable(evp_cipher evp_cipher.cpp)
target_link_libraries(evp_cipher OpenSSL::Crypto)

