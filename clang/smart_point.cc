#include <iostream>
#include <string>
#include <memory>

class bulb{
public:
	void turn_on(void) { status = "bulb on"; }
	void turn_off(void) { status = "bulb off"; }
	void print_state(void) { std::cout << status << std::endl; }

private:
	std::string status;
};

int main(int argc , char *argv[]){
	/*
	bulb b1;
	b1.turn_on();
	b1.print_state();

	b1.turn_off();
	b1.print_state();
	*/

	/*
	bulb *pb = new bulb();
	pb->turn_on();
	pb->print_state();

	pb->turn_off();
	pb->print_state();
	delete pb;
	*/

	/*
	std::unique_ptr<bulb> pb = std::make_unique<bulb>();
	pb->turn_on();
	pb->print_state();

	pb->turn_off();
	pb->print_state();
	return 0;
	*/

	auto pb = std::make_unique<bulb>();
	pb->turn_on();
	pb->print_state();

	pb->turn_off();
	pb->print_state();

	return 0;
}
