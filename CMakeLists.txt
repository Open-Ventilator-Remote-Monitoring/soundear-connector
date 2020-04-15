cmake_minimum_required(VERSION 3.5)
set(PROJECTNAME "se3connector")


project(${PROJECTNAME} LANGUAGES C CXX)



set (SOURCES 
    main.cpp 
    se3device.cpp 
    Sockets/filesocket.cpp 
    Sockets/metersocket.cpp 
    hidapi/hid-libusb.c 
    Helpers/helpers.cpp 
    Helpers/leq.cpp 
    common.cpp 
    Helpers/leds.cpp )

set(HEADERS
    DeviceBase.hpp 
    se3device.hpp 
    ise3commands.hpp 
    Sockets/filesocket.hpp 
    Sockets/protocoldefs.hpp 
    Sockets/metersocket.hpp 
    Sockets/iprotocolinterface.hpp 
    hidapi/hidapi.h 
    Sockets/isocketbase.hpp 
    Observers/observers.hpp 
    Helpers/helpers.hpp 
    Helpers/leq.hpp 
    common.hpp 
    Helpers/leds.hpp 
    version.hpp)
 
add_executable(${PROJECTNAME} ${SOURCES}  ${HEADERS})

target_include_directories(${PROJECTNAME} PUBLIC .)
target_include_directories(${PROJECTNAME} PUBLIC ./hidapi)
target_include_directories(${PROJECTNAME} PUBLIC /opt/poky/2.3/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/)
target_include_directories(${PROJECTNAME} PUBLIC /opt/poky/2.3/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/c++/6.3.0/)
target_include_directories(${PROJECTNAME} PUBLIC /opt/poky/2.3/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi/usr/include/c++/6.3.0/arm-poky-linux-gnueabi/)

target_link_libraries(${PROJECTNAME} PRIVATE usb-1.0 pthread -lsystemd)
