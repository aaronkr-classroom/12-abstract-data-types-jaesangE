#ifndef GUARD_H_VEC
#define GUARD_H_VEC




#include<algorithm>//max�Լ�
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
	//������
	Vec() { create(); }
	explicit Vec(size_type n, const T& val = T()) {
		create(n, val);
	}
	Vec(const Vec& v) { create(v.begin(), v.end()); }
	Vec& operator=(const Vec&);
	
	//���� ũ��� �ε����� ���� ���ο� �Լ���
	size_type size() const { return limit - data; }
	T& operator [](size_type i) { return data[i]; }
	const T& operator [](size_type i) const { return data[i]; }
	//�ݺ��� �Լ�
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

//������ �Լ���(3��)
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
//�Ҹ��� ����Լ�
template <class T> void Vec<T>::uncreate() {
	if (data) {
		iterator it = avail;
		while (it != data)
			alloc.destroy(--it);

		alloc.deallocate(data, limit - data);
	}
	data = avail = limit = 0;
}


//�޸� ���� �ø��� �Լ�
template <class T> void Vec<T>::grow() {
	//���� ������ �ø����� ���� ����ϴ�
	//���� ������ 2�踦 �Ҵ�
	size_type new_size = max(2 * (limit - data), ptrdiff_t(1));

	//���ο� ���� ������ �Ҵ��ϰ� ���� ��ҵ��� ���ο� ��������� ����
	iterator new_date = alloc.allocate(new_size);
	iterator new_avial = uninitialized_copy(data, avail, new_data);

	//������ ���� ������ ��ȯ
	uncreate();

	//���Ӱ� �Ҵ��� ���� ������ ����Ű���� ������ �缳��
	data = new_data;
	avail = new_avail;
	limit = data + new_size;
}
//���ο� ��� �߰�
template <class T> void Vec<T>::unchecked_append(const T& val) {
	alloc.construct(avail++, val);
}
template <class T> Vec<T>& Vec<T>::operator=(const Vec& rhs) {
	// �ڰ� �Ҵ� ���� Ȯ��
	if (&rhs != this) {
		// �º��� ���� �迭�� �����ϴ� �޸𸮸� ����
		uncreate();
		//�캯�� ���� ��Ҹ� �º����� ����
		create(rhs.begin(), rhs.end());

		return *this;
	}
}

#endif