#include <stdlib.h>
#include <stdio.h>
#include <hello.h>

int main(int argc, char *argv[])
{
	const char *name;
	if (argc > 1)
		{
			name = argv[1];
		}
	else
		{
			name = "World";
		}
	greet(stdout, name);
	return EXIT_SUCCESS;
}
