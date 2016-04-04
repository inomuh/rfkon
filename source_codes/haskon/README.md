# HASKON

Haskon Web Service is created to send the HasKon data collected from evarobot to the KonSens.

## Prerequisites

Before you compile the source codes, you need to install couple of packages in your linux machine:

1.Boost

```
sudo apt-get install libboost-all-dev
```

2.Cmake

```
sudo apt-get install cmake
```

3.Casablanca

Note: Your g++ complier has to 4.8 or higher to compile the library

```
sudo apt-get install libboost1.54-all-dev libssl-dev
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

Then, install the library globally
```
sudo make install
```

if you have any problems about the casablanca, you check their github respository:
[Casablanca](https://github.com/Microsoft/cpprestsdk)

4.Json library

[Extremely simple JSON](http://www.codeproject.com/Articles/856277/ESJ-Extremely-Simple-JSON-for-Cplusplus) is already included in the project, so you don't have to do anything.


## Compilation

```
git clone https://github.com/inomuh/rfkon.git
cd rfkon/source_codes/haskon
mkdir build
cd build
cmake ..
```



