#include <boost\smart_ptr.hpp>
#include <iostream>

class A;
class B;

class A
{
public:
	A (int id)
	{
		this->id = id;
	}

	~A()
	{
		std::cout << "~A " << id << std::endl;
	}
	boost::shared_ptr<B> m_spB;
	int id;
};

void TestMain()
{
	boost::shared_ptr<A> spA(new A(1));
	// reset 和 NULL赋值效果一样
	spA = NULL;
	spA.reset(new A(2));
	//spA = NULL;
	std::cout << "testmain end" << std::endl;
}
