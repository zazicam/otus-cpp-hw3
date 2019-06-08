#pragma once

#include "pretty.h"

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

	logging_allocator() : count(0), buf(nullptr), cur(nullptr) {

	}

	~logging_allocator() = default;

	template<typename U> 
	logging_allocator(const logging_allocator<U, S>&) = delete;

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

	
