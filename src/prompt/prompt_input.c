#include "../../includes/minishell.h"

static void	prompt_action(char key, t_pos *pos, \
		t_dlist **cursor, t_shell *shell)
{
	if (key == CTRL_SPACE)
		select_mode(pos, cursor);
	else if (key == CTRLV)
		paste(pos, cursor, shell);
	else if (key == CTRLX)
		cut(pos, cursor, shell);
	else if (key == CTRLY)
		copy(pos, cursor, shell);
	else if (key == ESC)
		esc(pos, cursor, shell);
	else if (key == BKS)
		backspace(pos, cursor);
	else if (key == DEL)
		delete(pos, cursor);
}

int	prompt_input(t_dlist **cursor, char *ps, t_shell *shell)
{
	char	key;
	t_pos	pos;

	init_pos(&pos, ps);
	while (1)
	{
		read(0, &key, 1);
		prompt_action(key, &pos, cursor, shell);
		if (key == LF || key == CTRLC)
		{
			ft_putchar_fd('\n', 1);
			return (key);
		}
		else if (key == CTRLD)
		{
			if ((*cursor)->next == NULL && (*cursor)->prev == NULL)
				return (key);
			delete(&pos, cursor);
		}
		else if (ft_isprint(key) || key == TAB)
			insert(cursor, key, &pos);
	}
}
