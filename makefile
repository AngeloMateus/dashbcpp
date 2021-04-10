CC = clang++
CFLAGS = -lncurses -stdlib=libc++ -Wall 
INC    = -framework IOKit

program: dashbcpp.cpp
	$(CC) $(CFLAGS) $(INC) -o dashbcpp dashbcpp.cpp external/smc.cpp
