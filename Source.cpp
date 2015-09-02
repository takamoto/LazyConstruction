#include "LazyConstruction.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

struct NoCopyable{
	int i;

	NoCopyable(int i) : i(i) {};
	NoCopyable(const NoCopyable &) = delete;
	NoCopyable & operator=(const NoCopyable &) = delete;

	NoCopyable(NoCopyable && rhs) : i(rhs.i) {
		std::cout << "move constructor" << std::endl;
	};
	NoCopyable & operator=(NoCopyable && rhs) {
		std::cout << "move assignment operator" << std::endl;
		i = rhs.i;
		return *this;
	};
};

int main() {
	LazyConstruction<int> v1;
	v1.construct(1);
	assert(*v1 == 1);
	
	// LazyInit<int> v1_1;
	// v1_1 = v1;
	// assert(*v1_1 == 1);
	
	// LazyInit<int> v1_2(v1);
	// assert(*v1_2 == 1);

	/* ----------------------- */

	LazyConstruction<std::string> v2;
	v2.construct("ss");
	assert(*v2 == "ss");
	*v2 += "s";
	assert(*v2 == "sss");

	// const LazyInit<std::string> v2_1(v2);
	// v2_1.construct("sss");
	// assert(*v2_1 == "sss");
	// *v2_1 += "s";
	
	LazyConstruction<const std::string> v2_2;
	v2_2.construct("sss");
	assert(*v2_2 == "sss");
	// *v2_2 += "s";

	// LazyInit<const std::string> v2_3(v2_2);
	//v2_3 = v2_2;
	// assert(*v2_3 == "sss");

	// LazyConstruction<const std::string> v2_4(std::move(v2_2));
	// assert(*v2_4 == "sss");

	/* ----------------------- */

	LazyConstruction<NoCopyable> v3;
	v3.construct(3);
	assert(v3->i == 3);

	// LazyInit<NoCopyable> v3_1(v3);
	// LazyConstruction<NoCopyable> v3_1(std::move(v3));
	// assert(v3_1->i == 3);

	LazyConstruction<NoCopyable> v3_2;
	// v3_2 = v3_1;
	// v3_2 = std::move(v3_1);
	// assert(v3_2->i == 3);

	return 0;
}