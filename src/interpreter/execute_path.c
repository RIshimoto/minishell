#include "../../includes/minishell.h"

void	update_cmd_path(char **cmd_path, char *tmp)
{
	struct stat	sb;

	if (*cmd_path == NULL)
		*cmd_path = ft_strdup(tmp);
	else if (stat(tmp, &sb) == 0)
	{
		if (S_IXUSR & sb.st_mode)
		{
			free(*cmd_path);
			*cmd_path = ft_strdup(tmp);
		}
	}
}

static char	**ft_split_path(char *str)
{
	char	**result;
	char	*next;

	result = ft_calloc2(sizeof(char *), 1);
	next = ft_strchr(str, ':');
	while (next)
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

static void	find_command(char *cmd, char **cmd_path, char **dir_names)
{
	DIR				*dir;
	struct dirent	*dp;
	int				i;
	char			*tmp;

	i = -1;
	while (dir_names[++i] != NULL)
	{
		dir = opendir(dir_names[i]);
		if (dir == NULL)
			continue ;
		dp = readdir(dir);
		while (dp != NULL)
		{
			if (ft_strcmp(cmd, dp->d_name) == EQUAL)
			{
				tmp = path_join(dir_names[i], dp->d_name);
				update_cmd_path(cmd_path, tmp);
				free(tmp);
			}
			dp = readdir(dir);
		}
		closedir(dir);
	}
}

char	*search_path(char *cmd_name, t_shell *shell)
{
	char	*env_value;
	char	**dir_names;
	char	*cmd_path;

	cmd_path = NULL;
	env_value = get_param_value(get_shell_var(shell, "PATH"));
	if (env_value == NULL || *env_value == '\0')
	{
		err_cstmmsg(cmd_name, NULL, strerror(ENOENT));
		shell->exit_status = 127;
		minishell_end(shell);
	}
	dir_names = ft_split_path(env_value);
	find_command(cmd_name, &cmd_path, dir_names);
	dp_free(dir_names);
	return (cmd_path);
}
