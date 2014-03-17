#include <stdio.h>
#include <hello.h>

void
greet(FILE *file, const char *name)
{
	fprintf(file, "Hello, %s!\n", name);
}
