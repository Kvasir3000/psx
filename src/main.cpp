#include <iostream>
#include <memory>
#include "../inc/psx.h" 


class Table;
class Table2
{
public:
	Table2(int t) : var(t) {};
	int get() { return var; };
	void foo() { std::cout << "Table 2 FOO\n"; };
	void fee() { std::cout << "Table 2 FEE\n"; };
private:
	int var;
};

template<typename C>
class Decoder
{
public: 
	Decoder(C* c) : var(c->get()), c(c) {
		clbk[0] = &C::foo;
		clbk[1] = &C::fee;
	};
	int getVar() { return var; };
	void execute(int idx)
	{
		(c->*clbk[idx])();
	}

private: 
	std::array<void(C::*)(), 2 > clbk;
	int var;
	C* c;
};

class Table
{
public:
	Table(int t) : var(t) {};
	int get() { return var; };
	void foo() { std::cout << "Table FOO\n"; };
	void fee() { std::cout << "Table FEE\n"; };
private:
	int var;
};

class Test2
{
private:
	std::string s2 = "Test";

	friend class Test;
};

class Test
{
	void push()
	{
		t2.s2;
	}
private:
	Test2 t2;
	std::string s = "Test";
	int g = 23;
};

int main()
{
	Table t(23);
	Table2 t2(55);
	Decoder<Table> d(&t);
	Decoder<Table2> d2(&t2);
	d.execute(0);
	d.execute(1);
	d2.execute(0);
	d2.execute(1);
	int var = d.getVar();
	int var2 = d2.getVar();
	void* ptr = nullptr;
	if (ptr)
	{
		std::cout << " log 1\n";
	}
	ptr = new int;
	if (ptr)
	{
		std::cout << " log 2\n";
	}
	std::unique_ptr<psx::PlayStation> ps1(new psx::PlayStation);
	ps1->run();
	return 0;
} 