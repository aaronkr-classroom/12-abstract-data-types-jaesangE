#ifndef GUARD_H_VEC
#define GUARD_H_VEC




#include<algorithm>//max함수
#include <memory> //allocator
#include <cstddef>//size_t, ptrdiff_t

using namespace std;
template <class T> class Vec {
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef T& reference;
	typedef const T& const_reference;
	//생성자
	Vec() { create(); }
	explicit Vec(size_type n, const T& val = T()) {
		create(n, val);
	}
	Vec(const Vec& v) { create(v.begin(), v.end()); }
	Vec& operator=(const Vec&);
	
	//벡터 크기와 인덱스에 관한 새로운 함수들
	size_type size() const { return limit - data; }
	T& operator [](size_type i) { return data[i]; }
	const T& operator [](size_type i) const { return data[i]; }
	//반복자 함수
	iterator begin() { return data; }
	const_iterator begin() { return data; }

	iterator end() { return avail; }
	const_iterator end() const { return avail; }

	void push_back(const T& val) {
		if (avail == limit)
			grow();

		unchecked_append(val);
	}
	-Vec() { uncreate(); }
	void clear() { uncreate(); }
	bool empty() { return data == 0; }
private:

	iterator limit;
	iterator avail;
	iterator data;

	std::allocator<T> alloc;
	void create();
	void create(size_type, const T&);
	void create(const_iterator, const_iterator);
	void uncreate();
	void grow();
	void unchecked_append(const T& val);
};

//생성자 함수들(3개)
template <class T> void Vec<T>::create() {
	data = avail = limit = 0;
}
template <class T> void Vec<T>::create(size_type n, const T& val) {
	data = alloc.allocate(n);
	limit = avai = data + n;
	uninitialized_fill(data, limit, val);
}

template <class T> void Vec<T>::create(const_iterator i, const_iterator j) {
	data = alloc.allocate(j - i);
	limit = avail = uninitailized_copy(i, j, data);
}
//소멸자 멤버함수
template <class T> void Vec<T>::uncreate() {
	if (data) {
		iterator it = avail;
		while (it != data)
			alloc.destroy(--it);

		alloc.deallocate(data, limit - data);
	}
	data = avail = limit = 0;
}


//메모리 공간 늘리는 함수
template <class T> void Vec<T>::grow() {
	//저장 공간을 늘리때는 현재 사용하는
	//저장 공간의 2배를 할당
	size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

	//새로운 저장 공간을 할당하고 기존 요소들을 새로운 저장공간에 복사
	iterator new_date = alloc.allocate(new_size);
	iterator new_avial = uninitialized_copy(data, avail, new_data);

	//기존의 저장 공간을 반환
	uncreate();

	//새롭게 할당한 저장 공간을 가리키려고 포인터 재설정
	data = new_data;
	avail = new_avail;
	limit = data + new_size;
}
//새로운 요소 추가
template <class T> void Vec<T>::unchecked_append(const T& val) {
	alloc.construct(avail++, val);
}
template <class T> Vec<T>& Vec<T>::operator=(const Vec& rhs) {
	// 자가 할당 여부 확인
	if (&rhs != this) {
		// 좌변이 지닌 배열이 점유하는 메모리를 해제
		uncreate();
		//우변이 지닌 요소를 좌변으로 복사
		create(rhs.begin(), rhs.end());

		return *this;
	}
}

#endif