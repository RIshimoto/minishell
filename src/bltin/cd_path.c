#include "../../includes/minishell.h"

static char	*get_parent(char *path)
{
	char	*result;

	result = ft_substr(path, 0, ft_strrchr(path, '/') - path);
	if (result && ft_strlen(result) == 0)
	{
		free(result);
		result = ft_strdup("/");
	}
	return (result);
}

void	update_path(char **result_path, char *original_path)
{
	char	*tmp;

	if (ft_strncmp(original_path, "..", 2) == 0)
	{
		tmp = *result_path;
		*result_path = get_parent(*result_path);
		free(tmp);
	}
	else if (original_path[0] != '.')
	{
		tmp = *result_path;
		*result_path = path_join(*result_path, original_path);
		free(tmp);
	}
}

void	update_path_join(char **result_path, char *path, char *new_path)
{
	char	*tmp;

	tmp = new_path;
	*result_path = path_join(path, new_path);
	free(tmp);
}

static char	*normalize(char *path)
{
	char		**str;
	int			i;
	char		*result;
	struct stat	buf;

	i = -1;
	str = ft_split(path, '/');
	result = ft_strdup("/");
	while (str[++i])
	{
		update_path(&result, str[i]);
		if (stat(result, &buf) != 0)
		{
			free(result);
			dp_free(str);
			return (NULL);
		}
	}
	if (path[0] == '/' && path[1] == '/' && path[2] != '/')
		update_path_join(&result, "/", result);
	dp_free(str);
	return (result);
}

char	*get_absolute_path(char *path, bool *is_absolute_path, t_shell *shell)
{
	char	*new_path;
	char	*norm_path;

	if (path[0] == '/')
		new_path = ft_strdup(path);
	else
		new_path = path_join(shell->pwd, path);
	norm_path = normalize(new_path);
	if (norm_path != NULL)
	{
		*is_absolute_path = true;
		free(new_path);
		return (norm_path);
	}
	*is_absolute_path = false;
	free(norm_path);
	return (new_path);
}
