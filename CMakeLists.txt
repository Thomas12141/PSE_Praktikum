cmake_minimum_required(VERSION 3.2)
project(pse_webserver LANGUAGES C VERSION 1.0.0)
set(PROJECT_DESCRIPTION "PSE Webserver PG 5")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, using DEBUG")
    set(CMAKE_BUILD_TYPE "DEBUG")
endif ()


add_executable(pse_webserver echo_server.c httplib.c httplib.h validation.c validation.h strlib.c strlib.h config.h http_status.h filelib.c filelib.h base64.c)
set_property(TARGET pse_webserver PROPERTY C_STANDARD 11)
install(TARGETS pse_webserver DESTINATION bin)

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

target_link_libraries(pse_webserver OpenSSL::Crypto)