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

