#/bin/sh

sudo update-alternatives --remove-all gcc
sudo update-alternatives --remove-all g++

sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install ninja-build cmake gcc-10 g++-10

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 50
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 50


ninja --version
cmake --version
gcc --version