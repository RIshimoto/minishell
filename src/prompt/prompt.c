#include "../../includes/minishell.h"

static int	outc(int c)
{
	return (write(1, &c, 1));
}

void	term_mode(char *p, int arg_cols, int arg_rows)
{
	char	buf[BUFFER_SIZE];
	char	*term;
	char	*cptr;
	int		n;

	term = getenv("TERM");
	if (term == NULL)
	{
		ft_putendl_fd("Error: getenv", 2);
		exit(1);
	}
	if (tgetent(NULL, term) != 1)
	{
		ft_putendl_fd("Error: tgetent", 2);
		exit(1);
	}
	cptr = buf;
	n = tgetnum(p);
	if (tgetstr(p, &cptr) != NULL)
		tputs(tgoto(buf, arg_cols, arg_rows), 1, outc);
	else if (n != -1)
		(void)printf("%d\n", n);
}

static int	prompt_loop(char *ps, t_dlist **line, t_shell *shell)
{
	int	ret;

	ret = -1;
	while (ret != LF)
	{
		*line = ft_dlstnew(NULL);
		ft_putstr_fd(ps, 2);
		term_mode("im", -1, -1);
		ret = prompt_input(line, ps, shell);
		term_mode("ei", -1, -1);
		if (ret == CTRLC || ret == CTRLD)
		{
			*line = ft_dlsttop(*line);
			ft_dlstclear(line, free);
			if (ret == CTRLC)
				shell->exit_status = 1;
			else if (ret == CTRLD)
				return (CTRLD);
		}
	}
	return (LF);
}

void	prompt(char *ps, t_dlist **line, t_shell *shell)
{
	struct termios	tty;
	struct termios	tty_save;
	int				ret;
	t_dlist			*tmp;

	tcgetattr(STDIN_FILENO, &tty);
	tty_save = tty;
	tty.c_lflag ^= ((ICANON | ISIG | ECHO) & ~ECHONL);
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	ret = prompt_loop(ps, line, shell);
	tcsetattr(STDIN_FILENO, TCSANOW, &tty_save);
	if (ret == CTRLD)
	{
		ft_putendl_fd("exit", 2);
		ft_dlstclear(line, free);
		minishell_end(shell);
	}
	else if (ret == LF)
	{
		tmp = ft_dlsttop(*line);
		*line = tmp->next;
		ft_dlstdelone(tmp, free);
		save_history(*line, shell);
	}
}
