#include "../../includes/minishell.h"

void	move_to_rg(t_pos *pos, t_dlist **cursor)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
		return ;
	if (pos->cursor < pos->max_rg)
	{
		*cursor = (*cursor)->next;
		pos->cursor++;
		if (pos->cursor % ws.ws_col == 0)
			term_mode("do", -1, -1);
		else
			term_mode("nd", -1, -1);
	}
}

void	move_to_lf(t_pos *pos, t_dlist **cursor)
{
	struct winsize	ws;
	int				i;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
		return ;
	if (pos->max_lf < pos->cursor)
	{
		*cursor = (*cursor)->prev;
		pos->cursor--;
		if ((pos->cursor + 1) % ws.ws_col == 0)
		{
			term_mode("up", -1, -1);
			i = -1;
			while (++i < ws.ws_col)
				term_mode("nd", -1, -1);
		}
		else
			term_mode("le", -1, -1);
	}
}

void	move_to_up(t_pos *pos, t_dlist **cursor)
{
	struct winsize	ws;
	int				i;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
		return ;
	if (pos->cursor - ws.ws_col > pos->max_lf)
	{
		i = 0;
		while (i < ws.ws_col)
		{
			move_to_lf(pos, cursor);
			i++;
		}
	}
}

void	move_to_down(t_pos *pos, t_dlist **cursor)
{
	struct winsize	ws;
	int				i;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
		return ;
	if (pos->cursor + ws.ws_col < pos->max_rg)
	{
		i = 0;
		while (i < ws.ws_col)
		{
			move_to_rg(pos, cursor);
			i++;
		}
	}
}
