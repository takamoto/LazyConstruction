#pragma once
#include <type_traits>
#include <cassert>

template<typename T>
class LazyInit{
public:
	LazyInit() = default;
	LazyInit(const LazyInit & rhs) { if(rhs) construct(*rhs); };
	LazyInit(LazyInit && rhs) { if(rhs) construct(std::move(*rhs)); };
	LazyInit & operator=(LazyInit rhs) {
		if (rhs) {
			if (isConstructed()) **this = std::move(*rhs);
			else construct(std::move(*rhs));
		} else {
			if (isConstructed()) reinterpret_cast<T*>(&_seat)->~T();
		}
		return *this;
	};
	
	~LazyInit() { if(isConstructed()) reinterpret_cast<T*>(&_seat)->~T(); }
	
	T * operator->() { return reinterpret_cast<T*>(&_seat); }
	T & operator*() { return *reinterpret_cast<T*>(&_seat); }
	const T * operator->() const { return reinterpret_cast<const T*>(&_seat); }
	const T & operator*() const { return *reinterpret_cast<const T*>(&_seat); }

	template<typename ...Args>
	void construct(Args&&... args) {
		assert(!isConstructed());
		new(&_seat) T(std::forward<Args>(args)...);
		_constructed = true;
	}

	bool isConstructed() const { return _constructed; }
	explicit operator bool() const { return isConstructed(); }

private:
	bool _constructed = false;
	typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type _seat;
};