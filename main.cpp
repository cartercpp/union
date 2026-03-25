#include <iostream>
#include "custom_union.hpp"

int main()
{
	custom_union<int, double> numbers(6.9);
	numbers = 67;
	numbers = 4.20;
}
