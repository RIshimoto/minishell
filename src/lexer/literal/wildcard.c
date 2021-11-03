#include "../../../includes/minishell.h"

static bool	match(char *s1, char *s2)
{
	if (s1[0] == '\0' && s2[0] == '\0')
		return (true);
	if (s1[0] == '*')
	{
		if (s2[0] == '\0')
			return (match(&s1[1], &s2[0]));
		else
		{
			if (match(&s1[1], &s2[0]) == true)
				return (true);
			else
				return (match(&s1[0], &s2[1]));
		}
	}
	if (s1[0] == s2[0])
		return (match(&s1[1], &s2[1]));
	return (false);
}

static char	**ft_dstrjoin(char **src, char **dest)
{
	char	**result;
	int		i;
	int		j;

	result = ft_calloc2(sizeof(char), 1);
	if (src != NULL)
	{
		i = 0;
		while (src[i] != NULL)
		{
			result = ft_realloc2(result, ft_strdup(src[i]));
			i++;
		}
		free(src);
	}
	if (dest != NULL)
	{	
		j = -1;
		while (dest[++j] != NULL)
			result = ft_realloc2(result, ft_strdup(dest[j]));
		free(dest);
	}
	return (result);
}

static void	save_store(char ***store, struct dirent *dp, char **str, char *dty)
{
	char		*s;
	char		*tmp;
	struct stat	st;

	if (stat(dp->d_name, &st) == 0)
	{
		if ((st.st_mode & S_IFMT) == S_IFDIR)
			*store = ft_dstrjoin(*store, recursive(dp->d_name, str + 1));
		else
		{
			tmp = ft_strjoin(dty, "/");
			s = ft_strjoin(tmp, dp->d_name);
			free(tmp);
			*store = ft_realloc2(*store, s);
		}
	}
}

char	**recursive(char *dty, char **str)
{
	DIR				*dir;
	struct dirent	*dp;
	char			**store;

	store = ft_calloc2(sizeof(char *), 1);
	dir = opendir(dty);
	if (dir == NULL)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (NULL);
	}
	dp = readdir(dir);
	while (dp != NULL)
	{
		if (dp->d_name[0] != '.' && match(*str, dp->d_name))
		{
			if (*(str + 1) == NULL)
				store = ft_realloc2(store, ft_strdup(dp->d_name));
			else
				save_store(&store, dp, str, dty);
		}
		dp = readdir(dir);
	}
	closedir(dir);
	return (store);
}

void	wildcard(t_ip *ip, t_list **tokens)
{
	char	**store;
	char	**str;
	int		i;

	str = ft_split(ip->id_string, '/');
	store = recursive(".", str);
	free(ip->id_string);
	dp_free(str);
	if (store == NULL)
		return ;
	sort(store);
	i = -1;
	while (store[++i] != NULL)
	{
		ip->sy = IDENTIFY;
		ip->id_string = store[i];
		if (store[i + 1] != NULL)
		{
			save_token(ip, tokens);
			free(ip->id_string);
		}
	}
	free(store);
}
