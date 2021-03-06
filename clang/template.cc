#include <cstdio>
#include <iostream>

#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

template <class T1>
void get_type(T1 &_type){
	T1 temp;
	temp = _type;
	std::cout << "SIZE : " << sizeof(temp) << std::endl;
}

template <class T1, class T2, class T3>
void get_types(T1 &t1, T2 &t2, T3 &t3){
	T1 temp1;
	T2 temp2;
	T3 temp3;
	std::cout << "SIZE temp1 : " << sizeof(temp1) << std::endl;
	std::cout << "SIZE temp2 : " << sizeof(temp2) << std::endl;
	std::cout << "SIZE temp3 : " << sizeof(temp3) << std::endl;
}

class Interpreter{
public:
    virtual void foo(){
        std::cout << "Virtual Test" << std::endl;
    }
};

class child : public Interpreter{
public:
    void foo() override{
        std::cout << "override done" << std::endl;
    }
};

int main(int argc, char *argv[]){
    //WARN_UNUSED_RESULT int unused;
	std::cout << "Hello World!" << std::endl;
	double v1;
	int v2;
	long v3;
	get_type(v1);
	get_type(v2);
	get_type(v3);

	get_types(v1, v2, v3);

    enum class eTest{ red, green };


    child *c1 = new child;
    std::cout << c1 << std::endl;
    c1->foo();

	return 0;
}
