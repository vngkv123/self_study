#include <iostream>
#include <cstdio>
#include <cstring>

template <class T>
void test(T &v1){
	std::cout << "Hello, World!" << std::endl;
	std::cout << v1 << std::endl;
}
namespace v8{
int root_v8 = 1683;
}

namespace _v8 {
namespace v8 {
int in_gv = 1337;
}
int out_gv = 7798;
void tf(){
	std::cout << v8::in_gv << std::endl;
	std::cout << ::v8::root_v8 << std::endl;
	std::cout << _v8::out_gv << std::endl;
}
}

int main(int argc, char *argv[]){
	std::cout << "main Function" << std::endl;
	_v8::tf();
	test("aSiagaming");
	return 0;
}
