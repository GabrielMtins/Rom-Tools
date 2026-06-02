#!/bin/sh

if [ ! -d ./build ]; then
	mkdir build && cd build
	#cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
	#cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..

	cd ..
fi

if [ $? -eq 0 ]; then
	project_name=$(grep "project(" CMakeLists.txt | sed "s/.*(//g; s/)//g")
	cd build && make -j16 && cd .. && ./build/"$project_name" 
fi
