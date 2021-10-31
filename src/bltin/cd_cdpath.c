#include "../../includes/minishell.h"

bool	is_use_cdpath(char **argv, char *path)
{
	if (argv[1] == NULL \
	|| ft_strcmp(path, ".") == 0 \
	|| ft_strcmp(path, "..") == 0 \
	|| argv[1][0] == '/' \
	|| ft_strncmp(path, "./", 2) == 0 \
	|| ft_strncmp(path, "../", 3) == 0 \
	|| getenv("CDPATH") == NULL)
		return (FALSE);
	return (TRUE);
}

static char	**split_cdpath(void)
{
	char	**str;
	char	*tmp;

	if (getenv("CDPATH")[0] == ':')
		tmp = ft_strjoin(".", getenv("CDPATH"));
	else
		tmp = ft_strdup(getenv("CDPATH"));
	str = ft_split(tmp, ':');
	free(tmp);
	return (str);
}

int	use_cdpath_chg(char *path, t_shell *shell)
{
	char	*cdpath;
	char	**str;
	int		i;

	i = 0;
	str = split_cdpath();
	while (str[i])
	{
		if (str[i][0] == '/')
			cdpath = path_join(str[i], path);
		else
			cdpath = three_path_join(shell->pwd, str[i], path);
		if (change_dir(cdpath, shell) == 0)
		{
			if (!(str[i][0] == '.' && str[i][1] == '\0'))
				ft_putendl_fd(shell->pwd, 1);
			dp_free(str);
			free(cdpath);
			return (TRUE);
		}
		free(cdpath);
		i++;
	}
	dp_free(str);
	return (FALSE);
}
