#include <stdio.h>
#include <hello.h>

int
greet(char *buf, size_t size, const char *name)
{
	return snprintf(buf, size, "Hello, %s!\n", name);
}
