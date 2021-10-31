#include "../../includes/minishell.h"

char	*get_home_path(char **argv)
{
	char	*path;

	if (argv[1] == NULL)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			err_cstmmsg("cd", NULL, "HOME not set");
			return (NULL);
		}
		return (path);
	}
	return (argv[1]);
}

char	*get_parent(char *path)
{
	char	*result;

	result = ft_substr(path, 0, ft_strrchr(path, '/') - path);
	if (result && ft_strlen(result) == 0)
		result = ft_strdup("/");
	return (result);
}

int	minishell_cd(char **argv, t_shell *shell)
{
	char	*new_path;
	int		status;

	if (!shell->pwd)
		shell->pwd = getcwd(NULL, 0);
	new_path = get_home_path(argv);
	if (ft_strcmp(new_path, "") == EQUAL)
		return (0);
	if (new_path == NULL)
		return (1);
	if (is_use_cdpath(argv, new_path) && use_cdpath_chg(new_path, shell))
		return (0);
	status = change_dir(new_path, shell);
	if (status == 0)
		return (0);
	err_errno("cd", new_path);
	return (1);
}
