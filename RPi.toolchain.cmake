# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   /opt/poky/2.3/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc CACHE FILEPATH "dev-gcc1")
SET(CMAKE_CXX_COMPILER /opt/poky/2.3/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++ CACHE FILEPATH "dev-g++1")
SET(CMAKE_AR /opt/poky/2.3/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-ar CACHE FILEPATH "dev-ar")
SET(CMAKE_RANLIB /opt/poky/2.3/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-ranlib CACHE FILEPATH "dev-ranlib")
# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  /opt/poky/2.3/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_SYSROOT /opt/poky/2.3/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi)

set(CMAKE_C_FLAGS  "-pipe -g -Wall -W -fPIC -march=armv7-a -mfloat-abi=hard --sysroot=${CMAKE_SYSROOT}" CACHE STRING "")
set(CMAKE_CXX_FLAGS   "-pipe -g -std=gnu++14 -Wall -W -fPIC -march=armv7-a -mfloat-abi=hard --sysroot=${CMAKE_SYSROOT}" CACHE STRING "")
 
