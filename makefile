CXX = x86_64-w64-mingw32-g++
CPPFLAGS = -g -std=c++17

SRCS = src/*.cpp src/glad.c sample_program/sample_program.cpp

RESOURCES_PATH = resources
DLL_FILES = dlls/glfw3.dll dlls/libassimp-5.dll
SHADER_PATH = shaders
INCLUDE_PATHS = -I./include -I./shaders
LIBRARY_PATHS = -L./lib

LINKER_FLAGS = -lglfw3dll -lassimp.dll

all: build

build: directory
	$(CXX) $(CPPFLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(SRCS) $(LINKER_FLAGS) -o out/sample_program.exe
	@cp $(DLL_FILES) out
	@cp -r $(RESOURCES_PATH) out
	@cp -r $(SHADER_PATH) out

directory:
	@mkdir -p out

clean:
	@rm -fr out
