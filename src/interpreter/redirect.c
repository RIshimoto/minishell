#include "../../includes/minishell.h"

static int	get_file_descriptor(char **rdt)
{
	int	n;

	if (!ft_isalnum(**rdt))
		return (INIT);
	n = 0;
	while (ft_isalnum(**rdt))
	{
		if (n > (INT_MAX - (**rdt - '0')) / 10)
			return (OUTOFRANGE);
		n = n * 10 + (**rdt - '0');
		(*rdt)++;
	}
	return (n);
}

int	redirect(t_queue *fds, t_shell *shell)
{
	int		n;
	int		result;
	char	*rdt;
	char	*file_name;
	char	**expand_file_name;

	result = SUCCESS;
	while (!q_empty(fds))
	{
		rdt = deq(fds);
		file_name = deq(fds);
		n = get_file_descriptor(&rdt);
		if (is_descriptor_err(n, shell) == true)
			return (FAIL);
		expand_file_name = expand_word(file_name, shell, true);
		if (is_ambiguous_err(expand_file_name, file_name, shell) == true)
			return (FAIL);
		if (ft_strcmp(rdt, "<<") == EQUAL)
			here_documents(file_name, shell);
		else
			result = without_here_documents(rdt, expand_file_name, n, shell);
		if (result == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}
