#include "../../includes/minishell.h"

void	handle_execve_error(char *cmd_path, t_shell *shell)
{
	struct stat	sb;

	if (errno == ENOENT)
		shell->exit_status = 127;
	else
		shell->exit_status = 126;
	stat(cmd_path, &sb);
	if (S_ISDIR(sb.st_mode))
	{
		err_cstmmsg(cmd_path, NULL, "is a directory");
		minishell_end(shell);
	}
	if (errno == ENOEXEC)
	{
		if ((S_IRUSR & sb.st_mode) && (S_IXUSR & sb.st_mode))
		{
			shell->exit_status = 0;
			minishell_end(shell);
		}
		errno = EACCES;
	}
	err_cstmmsg(cmd_path, NULL, strerror(errno));
	minishell_end(shell);
}

void	execute_cmds(char **args, t_shell *shell)
{
	extern char	**environ;
	char		*cmd;
	char		*cmd_path;

	cmd = args[0];
	if (ft_strchr(cmd, '/') != NULL)
		cmd_path = ft_strdup(cmd);
	else
	{
		cmd_path = search_path(cmd, shell);
		if (cmd_path == NULL)
			err_notfound(cmd, shell);
	}
	execve(cmd_path, args, environ);
	handle_execve_error(cmd_path, shell);
	free(cmd_path);
}
