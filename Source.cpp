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

struct Contain{
	Contain() {
		x.construct(3);
	}
	LazyConstruction<int> x;
};

int main() {
	LazyConstruction<int> v1;
	v1.construct(1);
	assert(*v1 == 1);
	
	LazyConstruction<int> v1_1;
	// v1_1 = v1; // runtime error : v1_1 is not constructed yet. 
	
	LazyConstruction<int> v1_2(v1);
	assert(*v1_2 == 1);

	/* ----------------------- */

	LazyConstruction<std::string> v2;
	v2.construct("ss");
	assert(*v2 == "ss");
	*v2 += "s";
	assert(*v2 == "sss");

	const LazyConstruction<std::string> v2_1(v2);
	// v2_1.construct("sss"); // compile error : v2_1 is constant
	// *v2_1 += "s";          // compile error : *v2_1 is constant
	
	LazyConstruction<const std::string> v2_2;
	v2_2.construct("sss");
	assert(*v2_2 == "sss");
	// *v2_2 += "s"; // compile error : *v2_2 is constant

	LazyConstruction<const std::string> v2_3(v2_2);
	// v2_3 = v2_2;  // compile error : *v2_3 is constant
	
	LazyConstruction<const std::string> v2_4(std::move(v2_2));
	assert(*v2_4 == "sss");

	/* ----------------------- */

	LazyConstruction<NoCopyable> v3;
	v3.construct(3);
	assert(v3->i == 3);

	// LazyConstruction<NoCopyable> v3_1(v3); // compile error : can't copy construction
	LazyConstruction<NoCopyable> v3_1(std::move(v3));
	assert(v3_1->i == 3);

	LazyConstruction<NoCopyable> v3_2;
	// v3_2 = v3_1;            // compile error : can't copy assignment
	// v3_2 = std::move(v3_1); // runtime error : v3_2 is not constructed yet. 

	/* ------------------------ */

	Contain v4;
	auto v4_1 = v4;
	
	Contain v4_2;
	v4_2 = v4;

	return 0;
}