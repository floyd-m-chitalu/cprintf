#include "c_printf.h"

int main(int argc, char const *argv[])
{
	/* test code */
	
	//c_printf(stdout, "test foo %d |a} %f |b} %s |g} done.", 10, 10.0, "hello world!");
	

	auto test_str = 
R"test_foo(
Hello!!
|g| :D 

|!|

Welcome to the c_printf project

My name is |rb|Floyd Mulenga Chitalu (jnr)|!| and I am the developer of this project.
|r!b|I hope|!| that my work will be usefull to you. 

|r!b!|Thank you|!|
 
To demonstrate:
|r| RED
|g!| GREEN
|b| BLUE
|!|

integer: %d
floating point: %f
string: %s

)test_foo";

	c_printf(stdout, test_str, 666, 6.66, "IT WORKS!");
	return 0;
}