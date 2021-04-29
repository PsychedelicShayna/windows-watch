# Could be gcc/g++
compiler = clang++

target_binary = watch.exe
binary_type = -m64

builds/$(target_binary): builds/objects/main.o
	$(compiler) builds/objects/main.o $(binary_type) -o builds/$(target_binary)

builds/objects/main.o: source/main.cpp
	mkdir -p builds/objects
	$(compiler) $(binary_type) -c source/main.cpp -o builds/objects/main.o
