#include "../../includes/minishell.h"

bool	is_descriptor_err(int n, t_shell *shell)
{
	char	*char_n;

	if (n > 256 || n == OUTOFRANGE)
	{
		errno = 9;
		if (n > 256)
		{
			char_n = ft_itoa(n);
			err_cstmmsg(char_n, NULL, strerror(errno));
			free(char_n);
		}
		else
			err_cstmmsg("file descriptor out of range", NULL, strerror(errno));
		shell->exit_status = 1;
		return (true);
	}
	return (false);
}

bool	is_ambiguous_err(char **expand_file_name, \
char *file_name, t_shell *shell)
{
	if (expand_file_name[0] == NULL || expand_file_name[1] != NULL)
	{
		err_cstmmsg(file_name, NULL, "ambiguous redirect");
		shell->exit_status = 1;
		return (true);
	}
	return (false);
}
