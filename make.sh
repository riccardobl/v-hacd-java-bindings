#!/bin/bash
set -e
# set -o xtrace

if [ "$VERSION" = "" -a "$GITHUB_REF" != "" ];
then
    export VERSION="`if [[ $GITHUB_REF == refs\/tags* ]]; then echo ${GITHUB_REF//refs\/tags\//}; fi`"
    if [ "$VERSION" = "" ];
    then
        branch="${GITHUB_REF//refs\/heads\//}"
        export VERSION="$branch-SNAPSHOT"
    fi
else
    VERSION="1.1"
fi

DEPLOY="false"
mkdir -p build
mkdir -p build/tests
rm -Rf build/tests/resources
cp -Rf resources build/tests/
if [ ! -f "build/bash_colors.sh" ];
then
    wget  -q https://raw.githubusercontent.com/maxtsepkov/bash_colors/738f82882672babfaa21a2c5e78097d9d8118f91/bash_colors.sh -O build/bash_colors.sh
fi
source build/bash_colors.sh



function cleanTMP {
    rm -Rf build/tmp
    mkdir -p build/tmp
}

function clean {
    rm -R build
    mkdir build   
}

function downloadIfRequired {
    if [ ! -d "build/vhacd" ]; 
    then
        download
    fi
}

function download {
    clr_green "Download VHACD"
    rm -f build/tmp/vhacd.zip
    wget -q  https://github.com/kmammou/v-hacd/archive/ded1fe468c3b9db82d7747cbb114751f29b73d84.zip -O build/tmp/vhacd.zip
    mkdir build/tmp/ext
    unzip -q build/tmp/vhacd.zip -d  build/tmp/ext
    mv build/tmp/ext/* build/vhacd
}

function setPlatformArch {
    PLATFORM=$1
    ARCH=$2
    if [ "$ARCH" = "" ]; then
      OUT_PATH="$PWD/build/lib/$PLATFORM"
    else
        OUT_PATH="$PWD/build/lib/$PLATFORM-$ARCH"
    fi
    mkdir -p $OUT_PATH
}

function buildLinux64 {
    buildLinux "x86-64"
}


function buildLinux32 {
    buildLinux "x86"
}

function buildLinux {
    downloadIfRequired
    setPlatformArch "linux" $1
    clr_green "Compile for $PLATFORM $ARCH..."
    arch_flag="-m64"
    if [ "$1" = "x86" ];
    then
        arch_flag="-m32"
    fi
    build_script="
    g++ -mtune=generic -fpermissive -U_FORTIFY_SOURCE -fPIC -O3 $arch_flag -shared
      -Ibuild/vhacd/src/VHACD_Lib/inc 
      -Ibuild/vhacd/src/VHACD_Lib/public
      -Inative/src -pthread 
       build/vhacd/src/VHACD_Lib/src/*.cpp
       native/src/*.cpp
       -Wl,-soname,vhacd.so -o $OUT_PATH/libvhacd.so  -lrt"
    clr_escape "$(echo $build_script)" $CLR_BOLD $CLR_BLUE
    $build_script
    if [ $? -ne 0 ]; then exit 1; fi

    clr_green "Compile Tests for $PLATFORM $ARCH..."
    tests=($(ls -d native/tests/*))
    for i in "${tests[@]}"
    do 
        name=`basename $i`
        
    
        ex_name="$name.$PLATFORM.$ARCH"
        clr_brown "+ $ex_name"
        build_script="g++ $arch_flag -fpermissive
        -Ibuild/vhacd/src/VHACD_Lib/inc
        -Ibuild/vhacd/src/VHACD_Lib/public
        -Inative/src -pthread -fPIC
        -Inative/tests/commons/
        -I$i -Wl,--as-needed 
        build/vhacd/src/VHACD_Lib/src/*.cpp 
        native/src/*.cpp 
        $i/*.cpp 
        -o build/tests/$ex_name -lrt " 
        clr_escape "$(echo $build_script)" $CLR_BOLD $CLR_BLUE
        $build_script
        if [ $? -ne 0 ]; then exit 1; fi
    done
}


function buildWindows {
    downloadIfRequired
    setPlatformArch "win32" $1
    clr_green "Compile for $PLATFORM $ARCH..."
    compiler="x86_64-w64-mingw32-g++"
    if [ "$1" = "x86" ];
    then
        compiler="i686-w64-mingw32-g++"
    fi
    build_script="
    $compiler -mtune=generic -fpermissive -fPIC -U_FORTIFY_SOURCE -O3 -DWIN32  -shared
      -Ibuild/vhacd/src/VHACD_Lib/inc 
      -Ibuild/vhacd/src/VHACD_Lib/public
      -Inative/ -static
       build/vhacd/src/VHACD_Lib/src/*.cpp
       native/src/*.cpp -Wp,-w 
       -Wl,-soname,vhacd.dll  -o $OUT_PATH/vhacd.dll"
    clr_escape "$(echo $build_script)" $CLR_BOLD $CLR_BLUE
    $build_script
    if [ $? -ne 0 ]; then exit 1; fi

    
    clr_green "Compile Tests for $PLATFORM $ARCH..."
    tests=($(ls -d native/tests/*))
    for i in "${tests[@]}"
    do 
        name=`basename $i`
    
        ex_name="$name.$PLATFORM.$ARCH.exe"
        clr_brown "+ $ex_name"
        build_script="$compiler  -fpermissive   -DWIN32
        -Ibuild/vhacd/src/VHACD_Lib/inc
        -Ibuild/vhacd/src/VHACD_Lib/public
        -Inative/src      
        -Inative/tests/commons/
        -I$i  -static -Wp,-w -Wl,--as-needed 
        build/vhacd/src/VHACD_Lib/src/*.cpp 
        native/src/*.cpp 
        $i/*.cpp
         -o build/tests/$ex_name"
        clr_escape "$(echo $build_script)" $CLR_BOLD $CLR_BLUE
        $build_script
        if [ $? -ne 0 ]; then exit 1; fi
    done
}

function buildWindows32 {
    buildWindows "x86"   
}

function buildWindows64 {
    buildWindows "x86-64"   
}

function buildJavaBindings {
    if [ "$JAVA_HOME" = "" ];
    then
        export JAVA_HOME=$(dirname $(dirname $(readlink -f $(which javac))))
        
    fi
    echo "Use JAVA_HOME $JAVA_HOME"

    if [ ! -f build/jnaerator.jar ];
    then
        clr_green "Download jnaerator..."
        wget -q https://github.com/riccardobl/JNAerator/archive/refs/heads/master.zip -O build/tmp/jnaerator.zip
        mkdir -p build/tmp/jnaerator-ext
        unzip -q build/tmp/jnaerator.zip -d build/tmp/jnaerator-ext/      
        cd build/tmp/jnaerator-ext/*/   
        clr_green "Build jnaerator..."
        mvn -DskipTests -q clean install  -Dmaven.javadoc.skip=true -Dmaven.compiler.source=1.7 -Dmaven.compiler.target=1.7
        cp jnaerator/target/jnaerator-*-shaded.jar ../../../jnaerator.jar
        cd ../../../../
    fi
    clr_green "Build java bindings..."
    mkdir -p build/release
    rm -Rf  build/tmp/libvhacd
    mkdir -p build/tmp/libvhacd
    cp -Rf build/lib/* build/tmp/libvhacd/
    cmd="`which java` 
    -jar build/jnaerator.jar 
    -mode Directory     
    -runtime JNA      
    -sizeAsLong
    -noComments
    -noRawBindings
    -preferJavac  
    -package 'vhacd.vhacd_native'
    -f
    -DBINDINGS=1
    -library vhacd
    native/src/VHACDNative.h

    -o build/tmp/libvhacd"
    clr_escape "$(echo $cmd)" $CLR_BOLD $CLR_BLUE
    $cmd
    if [ $? -ne 0 ]; then exit 1; fi
  
    rm -f build/release/vhacd-native.jar 
 
    mkdir -p build/tmp/bin
    mkdir -p build/tmp/j
        
        
    if [ ! -f  build/jna-platform.jar    ];
    then
        clr_green "Download JNA Platform..."
        wget -q https://repo1.maven.org/maven2/net/java/dev/jna/jna-platform/4.2.2/jna-platform-4.2.2.jar -O build/jna-platform.jar        
    fi    
    
    if [ ! -f  build/jna.jar    ];
    then
        clr_green "Download JNA..."
        wget -q https://repo1.maven.org/maven2/net/java/dev/jna/jna/4.2.2/jna-4.2.2.jar -O build/jna.jar        
    fi    
    
    unzip -q build/jna.jar -d build/tmp/j/
    rm -Rf build/tmp/j/META-INF 
    unzip -q build/jna-platform.jar -d build/tmp/j/
    rm -Rf build/tmp/j/META-INF 
    
    clr_green "Build vhacd-native-$VERSION-sources.jar..."
    cp -Rf build/tmp/libvhacd/* build/tmp/j/
    cp -Rf java/src/* build/tmp/j/
    cp -Rf build/tmp/j/* build/tmp/bin/
    
    find build/tmp/j -type f ! -name '*.java' -delete

    
    cmd="`which jar`
    cf
    build/release/vhacd-native-$VERSION-sources.jar  
     -C build/tmp/j .
    "
    clr_escape "$(echo $cmd)" $CLR_BOLD $CLR_BLUE
    $cmd
    if [ $? -ne 0 ]; then exit 1; fi
    
    clr_green "Build vhacd-native-$VERSION.jar..."
    find build/tmp/bin -type f -name '*.java' > build/tmp/java-src.txt
    cmd="`which javac` -source 1.7 -target 1.7 -Xlint:none -cp build/tmp/bin @build/tmp/java-src.txt"
    clr_escape "$(echo $cmd)" $CLR_BOLD $CLR_BLUE
    $cmd
    if [ $? -ne 0 ]; then exit 1; fi
    
    find build/tmp/bin -type f -name '*.java' -delete
    cmd="`which jar`
    cf
    build/release/vhacd-native-$VERSION.jar  
     -C build/tmp/bin .
    "
    clr_escape "$(echo $cmd)" $CLR_BOLD $CLR_BLUE
    $cmd
    if [ $? -ne 0 ]; then exit 1; fi

    clr_green "Compile Tests for java..."
    tests=($(ls -d java/tests/*))
    for i in "${tests[@]}"
    do 
        name=`basename $i`
        ex_name="$name.jar"
         clr_brown "+ $ex_name"
        rm -Rf build/tmp/jt
        mkdir -p build/tmp/jt
        cp -Rf  $i/* build/tmp/jt        
        cp -Rf  build/tmp/bin/* build/tmp/jt 
        find  build/tmp/jt -type f -name '*.java' > build/tmp/java-src.txt
        cmd="`which javac` -source 1.7 -target 1.7  -Xlint:none -cp build/tmp/jt @build/tmp/java-src.txt"
        clr_escape "$(echo $cmd)" $CLR_BOLD $CLR_BLUE
        $cmd
        if [ $? -ne 0 ]; then exit 1; fi
        
        cmd="`which jar`
        cf
        build/tests/$ex_name  
        -C  build/tmp/jt  .
        "
        clr_escape "$(echo $cmd)" $CLR_BOLD $CLR_BLUE
        $cmd
        if [ $? -ne 0 ]; then exit 1; fi
    done


    echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<project
  xmlns=\"http://maven.apache.org/POM/4.0.0\"
  xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"
  xsi:schemaLocation=\"http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd\">

  <modelVersion>4.0.0</modelVersion>
      <groupId>vhacd</groupId>
    <artifactId>vhacd-native</artifactId>
    <version>$VERSION</version>

  <name>vhacd-native</name>
  <description>Native bindings for vhacd</description>
  <url></url>
</project> "  > build/release/vhacd-native-$VERSION.pom  

}

function buildMac {
    downloadIfRequired
    setPlatformArch "darwin" ""
    clr_green "Compile for $PLATFORM..."
    build_script="
    g++ -mtune=generic -fpermissive -U_FORTIFY_SOURCE -fPIC -O3  -shared
      -Ibuild/vhacd/src/VHACD_Lib/inc 
      -Ibuild/vhacd/src/VHACD_Lib/public
      -Inative/src
       build/vhacd/src/VHACD_Lib/src/*.cpp
       native/src/*.cpp
        -o $OUT_PATH/libvhacd.dylib"
    clr_escape "$(echo $build_script)" $CLR_BOLD $CLR_BLUE
    $build_script
    if [ $? -ne 0 ]; then exit 1; fi

    clr_green "Compile Tests for $PLATFORM $ARCH..."
    tests=($(ls -d native/tests/*))
    for i in "${tests[@]}"
    do 
        name=`basename $i`
        
    
        ex_name="$name.$PLATFORM"
        clr_brown "+ $ex_name"
        build_script="g++  -fpermissive
        -Ibuild/vhacd/src/VHACD_Lib/inc
        -Ibuild/vhacd/src/VHACD_Lib/public
        -Inative/src -fPIC
        -Inative/tests/commons/
        -I$i 
        build/vhacd/src/VHACD_Lib/src/*.cpp 
        native/src/*.cpp 
        $i/*.cpp 
        -o build/tests/$ex_name" 
        clr_escape "$(echo $build_script)" $CLR_BOLD $CLR_BLUE
        $build_script
        if [ $? -ne 0 ]; then exit 1; fi
    done
}


function buildAll {
    buildLinux32 
    buildLinux64  
    buildWindows32
    buildWindows64
    buildJavaBindings
}

cleanTMP
if [ "$1" = "" ];
then
    echo "Usage: make.sh target"
    echo " - Targets: buildAll,buildWindows32,buildWindows64,buildLinux32,buildLinux64,buildJavaBindings,clean"
else 
    clr_magenta "Run $@..."
    $@
    clr_magenta "Build complete, results are stored in $PWD/build/"
fi
