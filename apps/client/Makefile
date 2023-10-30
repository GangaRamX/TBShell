CXX = g++
APP =i tbsh_client
OBJS = tbsh_client_daemon.o
CXX_FLAGS = -Wall -pedantic -ggdb -O2 -I../../include 

all: $(APP)

clean:
		@rm -f tbsh_client tbsh_client_daemon.o
.cpp.o:
		@$(CXX) $(CXX_FLAGS) -c -o $@ $<

$(APP): $(OBJS)
		@$(CXX) -Wall -pedantic -ggdb -O2 -o $@ $(OBJS)
