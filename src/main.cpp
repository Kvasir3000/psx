#include <iostream>
#include <memory>
#include "../inc/psx.h"

int main()
{
	std::unique_ptr<psx::PlayStation> ps1(new psx::PlayStation);
	ps1->run();
	return 0;
}