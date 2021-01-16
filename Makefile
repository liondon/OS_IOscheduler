iosched: iosched.cpp
	g++ -std=c++17 -g iosched.cpp -o iosched 

clean: 
	rm -f iosched *~  