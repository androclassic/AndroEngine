echo  "Create build directory"
mkdir build
cd ./build

echo  "Generate Project"
if hash cmake 2>/dev/null; then
    cmake ../
else
    echo "\"cmake\" is not installed!"
fi
cd ../

echo  "Copy dll files"
mkdir ./bin/Debug/data
mkdir ./bin/Release/data


copy ./data/* ./bin/Debug/data/
copy ./AndroSDK/PowerVRSDK/DLL/* ./bin/Debug/

copy ./data/* ./bin/Release/data/
copy ./AndroSDK/PowerVRSDK/DLL/* ./bin/Release/

cd ./build
if hash make 2>/dev/null; then
    make
else
    echo "\"make\" is not installed!"
fi
