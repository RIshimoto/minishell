#include "../includes/minishell.h"

int main(int argc, char **argv)
{
	int i;

	i = 0;
	printf("call unset\n");
	while (i < argc)
	{
		printf("argv[%d]: %s\n", i, argv[i]);
		i++;
	}
	exit(EXIT_SUCCESS);
}
