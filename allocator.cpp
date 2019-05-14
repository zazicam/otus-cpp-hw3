#include "pretty.h"

#include <iostream>
#include <vector>
#include <map>

#define USE_PRETTY 1

template<typename T, int S>
struct logging_allocator {
	using value_type = T;

	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	template<typename U>
	struct rebind {
		using other = logging_allocator<U, S>;
	};

	logging_allocator() {
		count = 0;
		cur = nullptr;
		buf = nullptr;
	}

	~logging_allocator() = default;

	template<typename U> 
	logging_allocator(const logging_allocator<U, S>&) throw() {

	}

	T *allocate(std::size_t n) {
#ifndef USE_PRETTY
		std::cout << "allocate: [n = " << n << "]" << std::endl;
#else
		std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
		char *tmp = cur;
		if(!buf)
		{
			buf = reinterpret_cast<char *>( std::malloc(S * sizeof(T)) );
			if (!buf)
			{
				std::cout<<"bad_alloc!\n";
				throw std::bad_alloc();
			}
			count += n;
			cur = buf + n * sizeof(T);
			tmp = buf;
		}
		else
		{
			cur += n * sizeof(T);
			count += n;
		}
		return reinterpret_cast<T *>(tmp);
	}

	void deallocate(T *p, std::size_t n) {
#ifndef USE_PRETTY
		std::cout << "deallocate: [n  = " << n << "] " << std::endl;
#else
		std::cout << __PRETTY_FUNCTION__ << "[n = " << n << "]" << std::endl;
#endif
		count -= n;
		if(count==0)
		{
			std::free(buf);
			std::cout << "free: [n  = " << S << "] " << std::endl;
		}
	}

	template<typename U, typename ...Args>
	void construct(U *p, Args &&...args) {
#ifndef USE_PRETTY
		std::cout << "construct" << std::endl;
#else
		std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
		new(p) U(std::forward<Args>(args)...);
		std::cout<<"new done\n";
	}

	void destroy(T *p) {
#ifndef USE_PRETTY
		std::cout << "destroy" << std::endl;
#else
		std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
		p->~T();
	}
	
private:
	int count;
	char *buf; 
	char *cur;   
};

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
