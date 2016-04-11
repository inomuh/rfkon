# HASKON

Haskon Web Service is created to send the HasKon data collected from evarobot to the KonSens.

## Prerequisites

Before you compile the source codes, you need to install and build couple of packages in your linux machine:

1.Boost

```
sudo apt-get install libboost1.54-all-dev
```

2.Cmake

```
sudo apt-get install cmake
```
3.gcc and g++ compliers

**Warning**: Your g++ complier version has to be 5 or higher to compile the library. If not, follow these commands:

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-5 g++-5
sudo update-alternatives --remove-all gcc
sudo update-alternatives --remove-all g++
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 50
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 50
```

And to check your version
```
g++ --version
```

4.Casablanca

```
sudo apt-get install libssl-dev
git clone https://github.com/Microsoft/cpprestsdk.git casablanca
cd casablanca/Release
mkdir build.debug
cd build.debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

To test the Casablanca whether is working or not

```
cd Binaries
./test_runner *_test.so
```

If nothing is wrong about the Casablanca, you can now install the library globally on your system 
```
sudo make install
```

if you have any problems about the casablanca, you check their github respository:
[Casablanca](https://github.com/Microsoft/cpprestsdk)

5.Json library

[Extremely simple JSON](http://www.codeproject.com/Articles/856277/ESJ-Extremely-Simple-JSON-for-Cplusplus) is already included in the project, so you don't have to do anything.


## Compilation

```
git clone https://github.com/inomuh/rfkon.git
cd rfkon/source_codes/haskon
mkdir build
cd build
cmake ..
make
sudo ./HasKon_WebService
```
Note: you have to run the application as a root since we scan the Wifi result as a root as well 
