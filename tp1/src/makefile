CONTEXT=sdl2
#CONTEXT=glfw3

CXXFLAGS += -DFENETRE_$(CONTEXT)
CXXFLAGS += -g -W -Wall -Wno-unused-parameter -Wvla -std=c++17 -pedantic # -Wno-deprecated-declarations
CXXFLAGS += $(shell pkg-config --cflags glew || echo -I/usr/local/include)
CXXFLAGS += $(shell pkg-config --cflags $(CONTEXT) || echo -I/usr/local/include/SDL2)

LDFLAGS += -g
LDFLAGS += $(shell pkg-config --libs glew || echo -I/usr/local/lib -lGLEW)
LDFLAGS += $(shell pkg-config --libs $(CONTEXT) || echo -I/usr/local/lib -lSDL2)
LDFLAGS += -lfreeimage

ifeq "$(shell uname)" "Darwin"
  LDFLAGS += -framework OpenGL
  ifeq "$(CONTEXT)" "glfw3"
    LDFLAGS += -lobjc -framework Foundation -framework Cocoa
  endif
endif

SRC=main
BUILD=./build-$(shell uname)
EXE=$(BUILD)/tp1.exe

SRC = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJ = $(addprefix $(BUILD)/, $(notdir $(SRC:.cpp=.o)))

exe : $(EXE)
run : exe
	$(EXE)
$(EXE) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o$@ $^ $(LDFLAGS)

$(BUILD)/%.o : %.cpp | $(BUILD)
	$(CC) $(CFLAGS) $(CXXFLAGS) -o $@ -c $<

$(BUILD)/%.o : */%.cpp | $(BUILD)
	$(CC) $(CFLAGS) $(CXXFLAGS) -o $@ -c $<

$(BUILD) :
	mkdir -p $@

# pour construire un projet en utilisant cmake (il faut ensuite aller dans '$(BUILD)' et y faire make)
cmake :
	mkdir -p $(BUILD)
	cd $(BUILD) && cmake ..

# nettoyage
clean :
	rm -rf $(BUILD)/

# pour créer le fichier à remettre dans Moodle
remise zip :
	make clean
	rm -f INF2705_remise_tp1.zip
	zip -r INF2705_remise_tp1.zip *.cpp *.h *.glsl makefile *.txt shaders
