#include "../includes/leaks.h"

void	end(void)
{
	system("/usr/bin/leaks minishell_leaks");
}
