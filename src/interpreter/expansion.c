#include "../../includes/minishell.h"

char	now_word(char **word)
{
	return (**word);
}

char	next_word(char **word)
{
	if (**word != '\0')
		(*word)++;
	return (**word);
}

char	**expand_word(char *word, t_shell *shell, bool quote)
{
	char	**args;
	char	*arg;
	char	ch;

	args = ft_calloc2(sizeof(char *), 1);
	arg = NULL;
	ch = now_word(&word);
	while (*word != '\0')
	{
		while (ch == ' ')
			ch = next_word(&word);
		if (quote && ch == '\'')
			expansion_single_quote(&word, &arg, &args, shell);
		else if (quote && ch == '\"')
			expansion_double_quote(&word, &arg, &args, shell);
		else if (ch == '$')
			expansion_dollar(&word, &arg, &args, shell);
		else
			str(&arg, &ch, &word);
		ch = now_word(&word);
	}
	args = ft_realloc2(args, arg);
	return (args);
}

char	**expand_words(t_list *words, t_shell *shell, bool quote)
{
	char	**args;
	char	**ret;
	int		i;

	args = ft_calloc2(sizeof(char *), 1);
	while (words != NULL)
	{
		ret = expand_word(words->content, shell, quote);
		i = 0;
		while (ret[i] != NULL)
		{
			args = ft_realloc2(args, ret[i]);
			i++;
		}
		words = words->next;
		free(ret);
	}
	return (args);
}
