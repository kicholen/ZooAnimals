python ../../../Development/oxygine-framework/tools/others/embed_folder_js.py -s ../data

mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE="C:/Program Files/Emscripten/emscripten/1.34.1/cmake/Modules/Platform/Emscripten.cmake" -G"Unix Makefiles" .. 
make
cd ..

pause