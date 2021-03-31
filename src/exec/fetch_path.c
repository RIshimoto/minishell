#include "../../includes/minishell.h"

static void find_command(char *cmd, char **cmd_path, char **dir_names)
{
	int				i;
	DIR				*dir;
	struct dirent	*dp;
	struct stat		sb;

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
				*cmd_path = ft_strjoin("/", dp->d_name);
				*cmd_path = ft_strjoin(dir_names[i], *cmd_path);
				return;
			}
		}
		closedir(dir);
	}
	return;
}

char *fetch_path(t_args args, t_shell_var *sv)
{
	char	*env_value;
	char	**dir_names;
	char	*cmd_path;

	///fprintf(stderr, "(*args)[0]:%s\n", (*args)[0]);
	env_value = get_shell_var(*sv, "PATH");
	dir_names = ft_split(env_value, ':');
	find_command((*args)[0], &cmd_path, dir_names);
	//fprintf(stderr, "cmd_path:%s\n", cmd_path);
	return (cmd_path);
}
