#include "../../includes/minishell.h"

void	bubble_sort(char **tab, int n)
{
	char	*temp;
	int		i;
	int		j;

	i = 0;
	while (i < n - 1)
	{
		j = n - 1;
		while (j > i)
		{
			if (ft_strcmp(tab[j], tab[j - 1]) < 0)
			{
				temp = tab[j - 1];
				tab[j - 1] = tab[j];
				tab[j] = temp;
			}
			j--;
		}
		i++;
	}
}

static int	strcmp_ignorecase(const char *s1, const char *s2)
{
	while (ft_tolower(*s1) == ft_tolower(*s2))
	{
		if (*s1 == '\0')
			return (0);
		++s1;
		++s2;
	}
	return (ft_tolower(*s1) - ft_tolower(*s2));
}

void	sort(char **tab)
{
	char	*tmp;
	int		i;
	int		j;
	int		n;

	n = 0;
	while (tab[n] != NULL)
		n++;
	i = -1;
	while (++i < n - 1)
	{
		j = n;
		while (--j > i)
		{
			if (strcmp_ignorecase(tab[j], tab[j - 1]) < 0)
			{
				tmp = tab[j - 1];
				tab[j - 1] = tab[j];
				tab[j] = tmp;
			}
		}
	}
}
