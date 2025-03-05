CC = g++

CFLAGS = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

HOME = $(shell cd)
CFLAGS += -I $(HOME)

ERRDIR = errors
STKDIR = stk_lib
ASMDIR = assembly
INPUTDIR = input
PROCDIR = proc
DISASMDIR = disassembly

TARGET = processor

SOURCES = main.cpp
ERR = errors/errors.cpp
STK = stk_lib/stk.cpp
INPUT = input/get_database.cpp input/input.cpp
ASM = assembly/assembly.cpp
PROC = proc/proc.cpp
DISASM = disassembly/disassembly.cpp

OBJECTS = $(SOURCES:%.cpp = $(OBJDIR)/%.o)
ERROBJ = $(ERR:$(ERRDIR)/%.cpp = $(OBJDIR)/%.o)
STKOBJ = $(STK:$(STKDIR)/%.cpp = $(OBJDIR)/%.o)
INPUTOBJ = $(INPUT:$(INPUTDIR)/%.cpp = $(OBJDIR)/%.o)
ASMOBJ = $(ASM:$(ASMDIR)/%.cpp = $(OBJDIR)/%.o)
PROCOBJ = $(PROC:$(PROCDIR)/%.cpp = $(OBJDIR)/%.o)
DISASMOBJ = $(DISASM:$(DISASMDIR)/%.cpp = $(OBJDIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(ERROBJ) $(STKOBJ) $(INPUTOBJ) $(ASMOBJ) $(PROCOBJ) $(DISASMOBJ) 
	$(CC) $^ -o $@ $(CFLAGS)

$(OBJDIR)/%.o : %.cpp
	$(CC) -c $^ - o $@ $(CFLAGS)

$(OBJDIR)/%.o : $(ERRDIR)/%.cpp
	$(CC) -c $^ - o $@ $(CFLAGS)

$(OBJDIR)/%.o : $(STKDIR)/%.cpp
	$(CC) -c $^ - o $@ $(CFLAGS)

$(OBJDIR)/%.o : $(INPUTDIR)/%.cpp
	$(CC) -c $^ - o $@ $(CFLAGS)

$(OBJDIR)/%.o : $(ASMDIR)/%.cpp
	$(CC) -c $^ - o $@ $(CFLAGS)

$(OBJDIR)/%.o : $(PROCDIR)/%.cpp
	$(CC) -c $^ - o $@ $(CFLAGS)

$(OBJDIR)/%.o : $(DISASMDIR)/%.cpp
	$(CC) -c $^ - o $@ $(CFLAGS)

clean:
	del -rf $(TARGET) $(OBJDIR)/*.o