#pragma once
#include <type_traits>
#include <cassert>

template<typename T>
class LazyConstruction{
public:
	LazyConstruction() = default;
	LazyConstruction(const LazyConstruction & rhs) { construct(*rhs); };
	LazyConstruction(LazyConstruction && rhs) { construct(std::move(*rhs)); };
	LazyConstruction & operator=(LazyConstruction && rhs) {
		**this = std::move(*rhs);
		return *this;
	};
	LazyConstruction & operator=(const LazyConstruction & rhs) {
		**this = *rhs;
		return *this;
	};
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

	bool isConstructed() const { return _constructed; }
	explicit operator bool() { return _constructed; }

private:
	bool _constructed = false;
	typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type _seat;
};