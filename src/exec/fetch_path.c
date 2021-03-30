#include "../../includes/minishell.h"

static bool find_command(char *cmd, char ***cmd_paths, char **dir_names)
{
	int				i;
	DIR				*dir;
	struct dirent	*dp;
	struct stat		sb;
	char			*cmd_path;

	i = -1;
	while (dir_names[++i] != NULL)
	{
		if ((dir = opendir(dir_names[i])) == NULL)
			continue;
		while ((dp = readdir(dir)) != NULL)
		{
			if (lstat(dp->d_name, &sb) > 0)
				continue;
			if (ft_strcmp(cmd, dp->d_name) == 0)
			{
				cmd_path = ft_strjoin("/", dp->d_name);
				cmd_path = ft_strjoin(dir_names[i], cmd_path);
				*cmd_paths = ft_realloc2(*cmd_paths, cmd_path); 
				return (true);
			}
		}
		closedir(dir);
	}
	return (false);
}

char **fetch_path(t_args args, t_shell_var *sv)
{
	char	*env_value;
	char	**dir_names;
	char	**cmd_paths;

	env_value = get_shell_var(*sv, "PATH");
	dir_names = ft_split(env_value, ':');
	cmd_paths = (char **)ft_calloc2(sizeof(char *), 1);
	find_command((*args)[0], &cmd_paths, dir_names);
	return (cmd_paths);
}

/*
char **fetch_path(t_args *args, t_shell_var *sv)
{
	char	*env_value;
	char	**dir_names;
	char	**cmd_paths;
	char	*command;
	int 	i;

	env_value = get_shell_var(*sv, "PATH");
	dir_names = ft_split(env_value, ':');
	cmd_paths = (char **)ft_calloc2(sizeof(char *), 1);
	i = 0;
	while ((*args)[i] != NULL)
	{
		command = (*args)[i][0];
		find_command(command, &cmd_paths, dir_names);
		i++;
	}
	return (cmd_paths);
}
*/
