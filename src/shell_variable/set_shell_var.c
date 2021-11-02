#include "../../includes/minishell.h"

char	*calc_new_param(char *param, t_list *lst, char *name)
{
	char	*tmp;
	char	*appended_value;
	char	*new_param;

	if (ft_strnstr(param, "+=", ft_strlen(param)) != NULL)
	{
		appended_value = ft_strjoin(get_param_value(lst->content), \
												get_param_value(param));
		new_param = ft_strjoin("=", appended_value);
		tmp = new_param;
		new_param = ft_strjoin(name, new_param);
		free(appended_value);
		free(tmp);
	}
	else
		new_param = ft_strdup(param);
	return (new_param);
}

void	set_shell_var(t_shell *this, char *param)
{
	t_list	*lst;
	char	*tmp;
	char	*name;
	int		h;

	name = get_param_name(param);
	h = hash(name);
	lst = this->var[h];
	while (lst != NULL)
	{
		if (ft_strncmp(lst->content, name, ft_strlen(name)) == EQUAL)
		{
			tmp = lst->content;
			lst->content = calc_new_param(param, lst, name);
			set_environ(this, name);
			free(tmp);
			free(name);
			return ;
		}
		lst = lst->next;
	}
	free(name);
	ft_lstadd_back(&this->var[h], ft_lstnew(ft_strdup(param)));
}
