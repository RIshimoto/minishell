#include "../../includes/minishell.h"

int	gt(char *file, int n, t_shell *shell)
{
	int	fd;

	if (n == INIT)
		n = STDOUT_FILENO;
	fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, S_IWRITE | S_IREAD);
	if (fd == -1)
	{
		perror("Error\n");
		shell->exit_status = 1;
		return (FAIL);
	}
	dup2(fd, n);
	return (SUCCESS);
}

int	dgt(char *file, int n, t_shell *shell)
{
	int	fd;

	if (n == INIT)
		n = STDOUT_FILENO;
	fd = open(file, O_WRONLY | O_APPEND | O_CREAT, S_IWRITE | S_IREAD);
	if (fd == -1)
	{
		perror("Error\n");
		shell->exit_status = 1;
		return (FAIL);
	}
	dup2(fd, n);
	return (SUCCESS);
}

int	lt(char *file, int n, t_shell *shell)
{
	int	fd;

	if (n == INIT)
		n = STDIN_FILENO;
	fd = open(file, O_RDWR);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		shell->exit_status = 1;
		return (FAIL);
	}
	dup2(fd, n);
	return (SUCCESS);
}

int	without_here_documents(char *rdt, \
char **expand_file_name, int n, t_shell *shell)
{
	int	result;

	if (ft_strcmp(rdt, ">>") == EQUAL)
		result = dgt(*expand_file_name, n, shell);
	else if (ft_strcmp(rdt, ">") == EQUAL)
		result = gt(*expand_file_name, n, shell);
	else if (ft_strcmp(rdt, "<") == EQUAL)
		result = lt(*expand_file_name, n, shell);
	else
		result = SUCCESS;
	dp_free(expand_file_name);
	return (result);
}
