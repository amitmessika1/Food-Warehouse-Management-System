all: clean bin/warehouse

bin/warehouse: bin/main.o bin/Action.o bin/Customer.o bin/Order.o bin/Parse.o bin/Volunteer.o bin/WareHouse.o 
	g++ -o bin/warehouse bin/main.o bin/Action.o bin/Customer.o bin/Order.o bin/Parse.o bin/Volunteer.o bin/WareHouse.o 

bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

bin/Customer.o: src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp

bin/Order.o: src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Order.o src/Order.cpp

bin/Parse.o: src/Parse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Parse.o src/Parse.cpp

bin/Volunteer.o: src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Volunteer.o src/Volunteer.cpp

bin/WareHouse.o: src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/WareHouse.o src/WareHouse.cpp

link:
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/warehouse bin/*.o

clean:
	rm -f bin/*.o
