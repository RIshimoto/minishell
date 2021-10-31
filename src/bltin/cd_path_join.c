#include "../../includes/minishell.h"

char	*path_join(char *path, char *new_path)
{
	char	*result_path;
	char	*tmp;

	if (ft_strlen(path) == 0 || path[ft_strlen(path) - 1] != '/')
		tmp = ft_strjoin(path, "/");
	else
		tmp = ft_strdup(path);
	result_path = ft_strjoin(tmp, new_path);
	free(tmp);
	return (result_path);
}

char	*three_path_join(char *s1, char *s2, char *s3)
{
	char	*str;
	char	*str2;

	str = path_join(s1, s2);
	str2 = path_join(str, s3);
	free(str);
	return (str2);
}
