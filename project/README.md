Dependecies installation:
=========================

- before please do
```sudo apt install make```
```sudo apt install clang``` - should be 18+ version!

- Installing boost
-- from current ./project folder do

```wget https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.gz```
```tar -xzvf boost_1_85_0.tar.gz```
```cd boost_1_85_0```
```./bootstrap```
```./b2```

- Installing libsodium
-- from current ./project folder do

```wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.20-stable.tar.gz```
```tar -xzvf libsodium-1.0.20-stable.tar.gz```
```cd libsodium-stable```
```./configure && make && sudo make install```
```sudo ldconfig```