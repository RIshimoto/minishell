#include "../../includes/minishell.h"

#define clr_right	printf("\033[0K") //カーソル位置からその行の右端までをクリア
#define left(x)		printf("\033[%dD", x) //カーソルを指定数だけ左に移動

void init_pos(t_pos *pos, char *ps)
{
	pos->max_lf = ft_strlen(ps);
	pos->cursor = ft_strlen(ps);
	pos->max_rg = ft_strlen(ps);
}

void del(t_pos *pos, t_dlist **cursor)
{
	t_dlist *save;

	if (pos->cursor > pos->max_lf)
	{
		write(1, "\b", 1);
		term_mode("dc");
		save = (*cursor)->prev;
		ft_dlstdelone(*cursor, free);
		*cursor = save;
		pos->cursor--;
		pos->max_rg--;
	}
}

void history_prev(t_pos *pos, t_dlist **cursor, t_shell *shell)
{
	t_dlist *dlst;
	char *line;
	char *s;
	int i;

	if ((*shell)->hist_lst->next == NULL)
		return;
	if (pos->cursor > pos->max_lf)
	{
		left(pos->cursor - pos->max_lf);
		clr_right;
		fflush(0);
		init_pos(pos, "minishell$ "); // * Causion "Use PS1"
	}
	(*shell)->hist_lst = (*shell)->hist_lst->next;
	line = (char *)(*shell)->hist_lst->content;
	if (line == NULL)
		return;
	dlst = ft_dlstnew(NULL);
	i = -1;
	while (line[++i] != '\0')
	{
		s = ft_calloc(sizeof(char), 2);
		s[0] = line[i];
		ft_putstr_fd(s, 1);
		pos->cursor++;
		pos->max_rg++;
		ft_dlstadd_back(&dlst, ft_dlstnew(s));
	}
	*cursor = dlst;
	while ((*cursor)->next != NULL)
		*cursor = (*cursor)->next;
}

void history_next(t_pos *pos, t_dlist **cursor, t_shell *shell)
{
	t_dlist *dlst;
	char *line;
	char *s;
	int i;

	if ((*shell)->hist_lst->prev == NULL)
		return;
	if (pos->cursor > pos->max_lf)
	{
		left(pos->cursor - pos->max_lf);
		clr_right;
		fflush(0);
		init_pos(pos, "minishell$ "); // * Causion "Use PS1"
	}
	(*shell)->hist_lst = (*shell)->hist_lst->prev;
	line = (char *)(*shell)->hist_lst->content;
	if (line == NULL)
	{
		*cursor = ft_dlstnew(NULL);
		return;
	}
	dlst = ft_dlstnew(NULL);
	i = -1;
	while (line[++i] != '\0')
	{
		s = ft_calloc(sizeof(char), 2);
		s[0] = line[i];
		ft_putstr_fd(s, 1);
		pos->cursor++;
		pos->max_rg++;
		ft_dlstadd_back(&dlst, ft_dlstnew(s));
	}
	*cursor = dlst;
	while ((*cursor)->next != NULL)
		*cursor = (*cursor)->next;
}

void esc(t_pos *pos, t_dlist **cursor, t_shell *shell)
{
	char	key;

	read(0, &key, 1);
	if (key == '[')
	{
		read(0, &key, 1);
		if(key == 'A')
			history_prev(pos, cursor, shell);
		if(key == 'B')
			history_next(pos, cursor, shell);
		if(key == 'C')
		{
			if(pos->cursor < pos->max_rg)
			{
				*cursor = (*cursor)->next;
				pos->cursor++;
				term_mode("nd");
			}
		}
		if(key == 'D')
		{
			if(pos->max_lf < pos->cursor)
			{
				*cursor = (*cursor)->prev;
				pos->cursor--;
				term_mode("le");
			}
		}
	}
}

void	insert(t_dlist **lst, char c, t_pos *pos)
{
	char	*s;
	t_dlist *new;

	s = ft_calloc(sizeof(char), 2);
	s[0] = c;
	new = ft_dlstnew(s);
	if (pos->cursor >= pos->max_rg)
		ft_dlstadd_back(lst, new);
	else
		ft_dlstinsert(lst, new);
	*lst = (*lst)->next;
	ft_putchar_fd(c, 1);
	pos->max_rg++;
	pos->cursor++;
}

void ctrl_d(t_pos *pos, t_dlist **cursor)
{
	if (pos->cursor < pos->max_rg)
	{
		ft_dlstdelone((*cursor)->next, free);
		term_mode("dc");
		pos->max_rg--;
	}
}
