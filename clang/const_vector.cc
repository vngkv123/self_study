#include <iostream>
#include <vector>
#include <memory>

class Functer{
public:
	void operator()(){
		std::cout << "ASFASDFASDF" << std::endl;
	}
};

class Point{
public:
	void pConst() const;
	int v1;
	int v2;
private:
	int x, y;
};

/* 	Declare that this is read-only object */

void Point::pConst() const{
	//this->x = 5;
	//this->y = 6;
	//this->v1 = 6;
	//this->v2 = 6;
	std::cout << "Point" << std::endl;
}

int main(int argc, char *argv[]){
	Functer func;
	func();
	std::vector<int> v1(1);
	std::vector<int> v2(5, 7);
	std::cout << v1[0] << std::endl;
	for( int i = 0; i < 5; i++ ){
		std::cout << v2[i] << std::endl;
	}
	return 0;
}
