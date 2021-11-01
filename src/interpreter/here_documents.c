#include "../../includes/minishell.h"

void	expand_line_content(char **word, t_dlist *line)
{
	while (line != NULL)
	{
		ft_charjoin(&(*word), *(char *)line->content);
		line = line->next;
	}
}

bool	finish_pfd(int pfd[2], t_dlist *tmp, char **content, char *end_word)
{
	if (ft_strcmp(*content, end_word) == EQUAL)
	{
		dup2(pfd[0], 0);
		dp_free(content);
		ft_dlstclear(&tmp, free);
		close(pfd[0]);
		close(pfd[1]);
		return (true);
	}
	return (false);
}

void	here_documents(char *end_word, t_shell *shell)
{
	t_dlist	*line;
	t_dlist	*tmp;
	char	*word;
	char	**content;
	int		pfd[2];

	pipe(pfd);
	while (1)
	{
		prompt("> ", &line, shell);
		tmp = line;
		word = ft_calloc(sizeof(char), 1);
		expand_line_content(&word, line);
		content = expand_word(word, shell, false);
		free(word);
		if (finish_pfd(pfd, tmp, content, end_word) == true)
			return ;
		ft_putendl_fd(*content, pfd[1]);
		dp_free(content);
		ft_dlstclear(&tmp, free);
	}
}
