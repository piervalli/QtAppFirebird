# QtAppFirebird
Firebird Android

QtAppFirebird is an app to test Firebird on Android with Qt 5.15.12
The driver for Firebird is not delivered by Qt, we need to build by source.

First we need to download source of Qt in the repository.
To build I used Jenkinks,  it is easy check the errors

The package "Firebird-5.0.0-android-arm64"  does not include headers  I have copied from
"Firebird-5.0.0-linux-arm64"
OS Version Ubuntu 20.04

Script of Jenkinks
```
#!/bin/bash
QT_VERSION=5.15.12
export QT_VERSION
BASE_BUILD=/home/pier/data/build
SOURCE_DIR=$BASE_BUILD/qt-everywhere-src-$QT_VERSION
JAVA_HOME=/home/pier/data/OpenJDK17U-jdk_x64_linux_hotspot_17.0.9_9/jdk-17.0.9+9
ANDROID_ABIS=arm64-v8a
export ANDROID_ABIS
ANDROID_SDK=/home/pier/Android/Sdk
export ANDROID_SDK
ANDROID_NDK=$ANDROID_SDK/ndk/22.1.7171670
export ANDROID_NDK
export JAVA_HOME
echo JAVA_HOME $JAVA_HOME
IBASE_PREFIX=$BASE_BUILD/Firebird-5.0.0-android-arm64/firebird
export IBASE_PREFIX
IBASE_INCDIR=$IBASE_PREFIX/include
export IBASE_INCDIR
BIND_DIR=$BASE_BUILD/$QT_VERSION/android_$ANDROID_ABIS
LD_LIBRARY_PATH=$BASE_BUILD/lib:$LD_LIBRARY_PATH:$IBASE_PREFIX
export LD_LIBRARY_PATH
export BIND_DIR
export PATH=$JAVA_HOME/bin:$PATH:IBASE_INCDIR
rm -r $BASE_BUILD/temp
rm -r $BASE_BUILD/$QT_VERSION/android_$ANDROID_ABIS
rm -r $BIND_DIR
mkdir $BASE_BUILD/$QT_VERSION
mkdir $BASE_BUILD/$QT_VERSION/android_$ANDROID_ABIS
mkdir $BASE_BUILD/bin
mkdir $BASE_BUILD/temp
mkdir $BASE_BUILD/$QT_VERSION
mkdir $BIND_DIR
cd $BASE_BUILD/temp

$SOURCE_DIR/configure -xplatform android-clang -verbose -recheck-all -shared -prefix $BIND_DIR -opensource -confirm-license -c++std c++17 -opengl es2 -disable-rpath -nomake tests -nomake examples -android-ndk $ANDROID_NDK -android-sdk $ANDROID_SDK -android-abis $ANDROID_ABIS -no-warnings-are-errors IBASE_PREFIX=$IBASE_PREFIX IBASE_INCDIR=$IBASE_INCDIR IBASE_LIBS=-lfbclient

make -j$(nproc)

make install

```

I used only the libirary for Arm64




```


```
