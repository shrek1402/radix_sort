CFLAGS = -std=c++17 -Werror -pedantic-errors -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wlogical-op -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo
OBJ = g++ $(CFLAGS) -c $< -o $@
TEST = # g++ $(CFLAGS) -I ..third/catch2 -c $< -o $@
allO = Radix_my_love.o CPUtime.o


##############################
all: build run               # 
##############################

build:  $(allFolders)

	
	
############################  BUILD  ############################

Radix: $(allO)
	g++ $(CFLAGS) $^ -o $@ 
	
Radix_my_love.o: Radix_my_love.cpp
	$(OBJ)

CPUtime.o: CPUtime.cpp
	$(OBJ)



	
############################  OTHER  ############################	
	
run: Radix
	
