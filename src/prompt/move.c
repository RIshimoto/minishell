#include "../../includes/minishell.h"

static void	move_to_next_word(t_pos *pos, t_dlist **cursor)
{
	if (pos->cursor >= pos->max_rg)
		return ;
	move_to_rg(pos, cursor);
	while (*(char *)(*cursor)->content == ' ')
	{
		move_to_rg(pos, cursor);
		if (pos->cursor >= pos->max_rg)
			return ;
	}
	while (*(char *)(*cursor)->content != ' ')
	{
		move_to_rg(pos, cursor);
		if (pos->cursor >= pos->max_rg)
			return ;
	}
	move_to_lf(pos, cursor);
}

static void	move_to_prev_word(t_pos *pos, t_dlist **cursor)
{
	if (pos->max_lf >= pos->cursor)
		return ;
	while (*(char *)(*cursor)->content == ' ')
	{
		move_to_lf(pos, cursor);
		if (pos->max_lf >= pos->cursor)
			return ;
	}
	while (*(char *)(*cursor)->content != ' ')
	{
		move_to_lf(pos, cursor);
		if (pos->max_lf >= pos->cursor)
			return ;
	}
}

void	move_to_word(t_pos *pos, t_dlist **cursor)
{
	char	key;

	read(0, &key, 1);
	if (key == ';')
	{
		read(0, &key, 1);
		if (key == '5')
		{
			read(0, &key, 1);
			if (key == 'A')
				move_to_up(pos, cursor);
			else if (key == 'B')
				move_to_down(pos, cursor);
			else if (key == 'C')
				move_to_next_word(pos, cursor);
			else if (key == 'D')
				move_to_prev_word(pos, cursor);
		}
	}
}

void	move_to_home(t_pos *pos, t_dlist **cursor)
{
	while (pos->max_lf < pos->cursor)
		move_to_lf(pos, cursor);
}

void	move_to_end(t_pos *pos, t_dlist **cursor)
{
	while (pos->cursor < pos->max_rg)
		move_to_rg(pos, cursor);
}
