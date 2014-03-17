#include <stdlib.h>
#include <stdio.h>
#include <hello.h>

int main(int argc, char *argv[])
{
	const char *name;
	char buf[80];
	int result;
	if (argc > 1)
		{
			name = argv[1];
		}
	else
		{
			name = "World";
		}
	result = greet(buf, sizeof(buf), name);
	if (result >= sizeof(buf))
		{
			return EXIT_FAILURE;
		}
	fputs(buf, stdout);
	return EXIT_SUCCESS;
}
