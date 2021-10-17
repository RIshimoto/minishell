#include "../../includes/minishell.h"

void error_message(char *s, char *path)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(s, 2);
}

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
		error_message("is a directory", cmd_path);
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
	error_message(strerror(errno), cmd_path);
	minishell_end(shell);
}

static void		find_command(char *cmd, char **cmd_path, char **dir_names)
{
	DIR				*dir;
	struct dirent	*dp;
	struct stat		sb;
	int				i;

	i = -1;
	while (dir_names[++i] != NULL)
	{
		if ((dir = opendir(dir_names[i])) == NULL)
			continue;
		dp = readdir(dir);
		while (dp != NULL)
		{
			if (ft_strcmp(cmd, dp->d_name) == EQUAL)
			{
				if (stat(dp->d_name, &sb) < 0)
				{
					*cmd_path = ft_strjoin(dir_names[i], "/");
					*cmd_path = ft_strjoin(*cmd_path, dp->d_name);
				}
				else
					*cmd_path = ft_strjoin(dir_names[i],dp->d_name);
				// execve
				return;
			}
			dp = readdir(dir);
		}
		closedir(dir);
	}
}

char **ft_split_path(char *str)
{
	char **result;
	char *next;

	result = ft_calloc2(sizeof(char*), 1);
	next = ft_strchr(str, ':');
	while(next)
	{
		if (next - str == 0)
			result = ft_realloc2(result, ft_strdup("./"));
		else
			result = ft_realloc2(result, ft_substr(str, 0, next - str));
		str = next + 1;
		next = ft_strchr(str, ':');
	}
	if (*str == '\0')
		result = ft_realloc2(result, ft_strdup("./"));
	else
		result = ft_realloc2(result, ft_strdup(str));
	return (result);
}

static char	*search_path(char *cmd_name, t_shell *shell)
{
	char	*env_value;
	char	**dir_names;
	char	*cmd_path;

	cmd_path = NULL;
	env_value = get_param_value(get_shell_var(shell, "PATH"));
	if (env_value == NULL || *env_value == '\0')
	{
		error_message(strerror(ENOENT), cmd_name);
		shell->exit_status = 127;
		minishell_end(shell);
	}
	dir_names = ft_split_path(env_value);
	find_command(cmd_name, &cmd_path, dir_names);
	dp_free(dir_names);
	return (cmd_path);
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
		if (errno && cmd_path)
			err_cstmmsg(cmd_path, NULL, strerror(errno));
		else
			err_notfound(cmd, shell);
	}
	execve(cmd_path, args, environ);
	handle_execve_error(cmd_path, shell);
	free(cmd_path);
}
