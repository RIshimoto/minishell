#include "../../includes/minishell.h"

int main(int argc, char **argv)
{
	int i;

	if (argc <= 1)
		return (0);
	while (argv[i])
	{
		ft_unsetenv(argv[i]);
		i++;
	}
	return (0);
}
