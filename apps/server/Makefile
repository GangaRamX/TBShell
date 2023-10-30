CXX = g++
APP = tbshell
OBJS = tbshell.o
CXX_FLAGS = -Wall -pedantic -ggdb -O2 -I../../include 

all: $(APP)

clean:
		@rm -f tbshell tbshell.o
.cpp.o:
		@$(CXX) $(CXX_FLAGS) -c -o $@ $<

$(APP): $(OBJS)
		@$(CXX) -Wall -pedantic -ggdb -O2 -o $@ $(OBJS)
