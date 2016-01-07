#include <boost\smart_ptr.hpp>
#include <iostream>

class A;
class B;

class A
{
public:
	~A()
	{
		std::cout << "~A()" << std::endl;
	}
	boost::shared_ptr<B> m_spB;
};

class B
{
public:
	~B()
	{
		std::cout << "~B()" << std::endl;
	}
	//boost::shared_ptr<A> m_spA;
	boost::weak_ptr<A> m_spA;
};

void TestMain()
{
	boost::shared_ptr<A> spA(new A);
	boost::shared_ptr<B> spB(new B);

	spA->m_spB = spB;
	spB->m_spA = spA;
}