#V-HACD Java Bindings
Java bindings for [V-HACD](https://github.com/kmammou/v-hacd)


##Build
The build script is compatible only with linux but it compiles for Windows as well, thanks to mingw32 compiler.

In order to run the build script, you'll need to install the following software 
```
wget unzip maven mingw32 build-essential oracle-jdk
```
Once you've installed these packages, you can launch the build with
```
./make.sh clean
./make.sh buildAll
```
Results are stored in: `build/tests` and `build/release`

##Gradle
```
repositories { 
    maven { 
        url "http://dl.bintray.com/riccardo/v-hacd" 
    } 
}

dependencies {
    compile "vhacd:vhacd-native:1.0.2"
}

```