#include <iostream>
#include <vector>
#include <map>

#include "allocator.hpp"

int main(int, char *[]) {
	
	const int SIZE = 10;
	auto m = std::map<int, int, std::less<int>, logging_allocator<std::pair<const int, int>, SIZE>>{};

	int p=1;
	for (int i = 0; i < SIZE; ++i) {
		std::cout << "vector size = " << m.size() << std::endl;
		if(i>0)
			p*=i;
		m[i] = p;
		std::cout << std::endl;
	}

	
	for (auto i: m) {
		std::cout << i.first <<" "<<i.second << std::endl;
	}

	std::cout << "\n**** end of main ****\n" << std::endl;

	return 0;
}
