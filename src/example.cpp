#include <iostream>
#include <assert.h>
#include <design-patterns-cpp14/factory.h>
#include <fast-event-system/async_fast.h>

int main()
{
	const int N = 5;
	int counter = 0;

	fes::async_fast<int> sync;
	sync.connect(sync);
	sync.connect([&counter](auto&) { 
		std::cout << "tick!" << std::endl;
		++counter;
	});

	sync(0);
	for (int i = 0; i < N; ++i)
	{
		sync.get();
	}

	assert(counter == N);
}


class Base
{
public:
	using factory = dp14::factory<Base, std::string, int>;

	explicit Base(const std::string& name, int q)
		: _name(name)
		  , _q(q)
	{
		std::cout << "constructor " << _name << " - " << _q << std::endl;
	}
	virtual ~Base() { std::cout << "destruction" << std::endl; }

protected:
	std::string _name;
	int _q;
};

class A : public Base
{
public:
	DEFINE_KEY(A)
		explicit A(const std::string& name, int q) : Base(name, q) { ; }
	virtual ~A() = default;
};
DEFINE_HASH(A)

class B : public Base
{
public:
	explicit B(const std::string& name, int q) : Base(name, q) { ; }
	virtual ~B() = default;
};

namespace std {
template <>
	struct hash<B>
	{
		size_t operator()() const
		{
			return std::hash<std::string>()("B");
		}
	};
}

int main2()
{
	Base::factory factory;
	Base::factory::registrator<A> reg1(factory);
	Base::factory::registrator<B> reg2(factory);

	{
		// equivalent ways of create A
		std::shared_ptr<Base> a1 = factory.create_specialized<A>("first parameter", 2);
		std::shared_ptr<A> a2 = factory.create_specialized<A>("first parameter", 2);
		std::shared_ptr<Base> a3 = factory.create("A", "first parameter", 2);

		// equivalent ways of create B
		std::shared_ptr<Base> b1 = factory.create_specialized<B>("first parameter", 2);
		std::shared_ptr<B> b2 = factory.create_specialized<B>("first parameter", 2);
		std::shared_ptr<Base> b3 = factory.create("B", "first parameter", 2);

		assert(a1 != a2);
		assert(a3 != b1);
		assert(b1 != b2);
	}

	return(0);
}
