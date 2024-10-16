CC = g++ 
DIR_O = objects
DIR_STACK = stack

CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

TARGET = myfile

SOURCES = main.cpp check.cpp stack.cpp assemb.cpp stack_operations.cpp
STACK_FILES = check.cpp stack.cpp stack_operations.cpp
STACK = $(addprefix $(DIR_STACK)/, $(STACK_FILES))
OTHER = main.cpp assemb.cpp

OBJECTS = $(addprefix $(DIR_O)/, $(SOURCES:.cpp=.o))

all: $(STACK) $(OTHER) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $^ 

clean:
	rm -rf *.o $(TARGET)
