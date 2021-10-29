#include "../../includes/minishell.h"

static void	print_env_name(char *str)
{
	int		i;

	i = 0;
	while (str[i] != 0)
	{
		if (ft_strchr("\'\"\\$`", str[i]) != NULL)
			ft_putstr_fd("\\", 1);
		ft_putchar_fd(str[i], 1);
		i++;
	}
}

static void	print_env_lines(char *tab)
{
	char	**str;
	int		i;

	ft_putstr_fd("declare -x ", 1);
	if (ft_strchr(tab, '=') == NULL)
		ft_putendl_fd(tab, 1);
	else
	{
		str = ft_split(tab, '=');
		ft_putstr_fd(str[0], 1);
		ft_putstr_fd("=\"", 1);
		if (str[1] != NULL)
		{
			i = 1;
			print_env_name(str[i++]);
			while (str[i] != NULL)
			{
				ft_putstr_fd("=", 1);
				ft_putstr_fd(str[i++], 1);
			}
		}
		ft_putendl_fd("\"", 1);
		dp_free(str);
	}
}

static void	show_env(void)
{
	extern char		**environ;
	char			**tab;
	int				i;

	tab = ft_calloc2(sizeof(char **), 1);
	i = 0;
	while (environ[i] != NULL)
	{
		tab = ft_realloc2(tab, environ[i]);
		i++;
	}
	bubble_sort(tab, i);
	i = 0;
	while (tab[i] != NULL)
	{
		print_env_lines(tab[i]);
		i++;
	}
	free(tab);
}

static int	set_env(char **argv, t_shell *shell)
{
	int		i;
	int		status;
	char	*name;

	i = 0;
	status = 0;
	while (argv[++i] != NULL)
	{
		if (!ft_isalpha(argv[i][0]) && !(argv[i][0] == '_' && \
ft_isalpha(argv[i][1])))
		{
			err_cstmmsg("export", argv[i], "not a valid identifier");
			status = 1;
		}
		else
		{
			if (ft_strchr(argv[i], '=') != NULL || \
get_shell_var(shell, argv[i]) == NULL)
				set_shell_var(shell, argv[i]);
			name = get_param_name(argv[i]);
			set_environ(shell, name);
			free(name);
		}
	}
	return (status);
}

int	minishell_export(char **argv, t_shell *shell)
{
	char	status;

	status = 0;
	if (argv[1] == NULL)
		show_env();
	else
		status = set_env(argv, shell);
	return (status);
}
