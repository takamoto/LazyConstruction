#pragma once
#include <type_traits>
#include <cassert>

template<typename T>
class LazyConstruction{
public:
	LazyConstruction() = default;
	LazyConstruction(const LazyConstruction & rhs) = delete;
	LazyConstruction(LazyConstruction && rhs) = delete;
	LazyConstruction & operator=(LazyConstruction rhs) = delete;
	~LazyConstruction() { if(_constructed) reinterpret_cast<T*>(&_seat)->~T(); }
	
	T * operator->() { assert(_constructed); return reinterpret_cast<T*>(&_seat); }
	T & operator*()  { assert(_constructed); return *reinterpret_cast<T*>(&_seat); }
	const T * operator->() const { assert(_constructed); return reinterpret_cast<const T*>(&_seat); }
	const T & operator*()  const { assert(_constructed); return *reinterpret_cast<const T*>(&_seat); }

	template<typename ...Args>
	void construct(Args&&... args) {
		assert(!_constructed);
		new(&_seat) T(std::forward<Args>(args)...);
		_constructed = true;
	}

private:
	bool _constructed = false;
	typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type _seat;
};