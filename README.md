# V-HACD Java Bindings
Java bindings for [V-HACD](https://github.com/kmammou/v-hacd)

[![Build Status](https://travis-ci.org/riccardobl/v-hacd-java-bindings.svg?branch=master)](https://travis-ci.org/riccardobl/v-hacd-java-bindings)

## Build 
### Dependencies
See .travis.yml 
___
### Build natives for linux and windows
From a linux installation launch:
```
./make.sh buildLinux32
./make.sh buildLinux64  
./make.sh buildWindows32
./make.sh buildWindows64
```
___
### Build natives for osx
From an osx installation launch:
```
./make.sh buildMac
```
___
### Build java bindings 
From an osx or linux installation launch:
```
./make.sh buildJavaBindings
```
___
### Build with travis
From an osx or linux instance launch:
```
./make.sh travis build
```


Results are stored in: `build/tests` and `build/release`

## Gradle
```
repositories { 
    maven { 
        url "http://dl.bintray.com/riccardo/v-hacd" 
    } 
}

dependencies {
    compile "vhacd:vhacd-native:${version}"
}

```
${version} should be changed with the [tag name](https://github.com/riccardobl/v-hacd-java-bindings/tags) of the release you want to use.
