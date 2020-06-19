CFLAGS = -std=c++17 -Werror -pedantic-errors -Wall -Wextra -Wpedantic -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wduplicated-branches -Wduplicated-cond -Wextra-semi -Wfloat-equal -Wlogical-op -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo
OBJ = g++ $(CFLAGS) -c $< -o $@
TEST = # g++ $(CFLAGS) -I ..third/catch2 -c $< -o $@
allO = Radix my love.o CPUtime.o


##############################
all: build test run-tests run# 
##############################

build: Radix my love

	
	
############################  BUILD  ############################

Radix: $(allO)
	g++ $(CFLAGS) $^ -o $@ 
	
Radix my love.o: Radix my love.cpp
	$(OBJ)

Radix my CPUtime.o: CPUtime.cpp
	$(OBJ)



	
############################  OTHER  ############################	
	
run: Radix
	
