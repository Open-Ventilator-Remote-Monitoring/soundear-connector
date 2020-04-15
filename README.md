# soundear-connector
C++ project for reading measurements from a SoundEar Noise meter connected on a USB port

This initial implementation just outputs measurements from the noise meter every second.

It is a CMake project with a toolchain file fitted for a custom Yocto Linux distribution - you will most likely need to change the RPi.toolchain.cmake to fit your cross compiler.

# Generate the build recipe 
From the root of the project type:
 ./generate-build.sh
 
# Build the application
From the root of the project type:
make
