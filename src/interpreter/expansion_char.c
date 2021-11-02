#include "../../includes/minishell.h"

void	expansion_dollar(char **word, char **arg, char ***args, t_shell *shell)
{
	char	*val;
	int		i;

	val = parameter(word, shell);
	if (val == NULL)
		return ;
	i = 0;
	while (val[i] != '\0')
	{
		if (ft_strchr(" \t\n", val[i]) != NULL)
		{
			*args = ft_realloc2(*args, *arg);
			*arg = NULL;
		}
		else
			ft_charjoin(arg, val[i]);
		i++;
	}
	free(val);
}

static bool	is_closed(char *word, char **arg)
{
	char	ch;
	char	end_ch;

	ch = next_word(&word);
	end_ch = now_word(&word);
	while (ch != end_ch)
	{
		if (ch == '\0')
		{
			ch = now_word(&word);
			ft_charjoin(arg, ch);
			return (false);
		}
		ch = next_word(&word);
	}
	return (true);
}

void	expansion_double_quote(char **word, char **arg, char ***args, t_shell *shell)
{
	char	ch;
	char	*tmp;

	tmp = *arg;
	*arg = ft_strjoin(*arg, "");
	free(tmp);
	is_closed(*word, arg);
	ch = next_word(word);
	while (ch != '\"')
	{
		if (ch == '\0')
			break ;
		if (ch == '$')
		{
			expansion_dollar(word, arg, args, shell);
			ch = now_word(word);
		}
		else
		{
			ft_charjoin(arg, ch);
			ch = next_word(word);
		}
	}
	next_word(word);
}

void	expansion_single_quote(char **word, char **arg, char ***args, t_shell *shell)
{
	char	ch;
	bool	quote;
	char	*tmp;

	tmp = *arg;
	*arg = ft_strjoin(*arg, "");
	free(tmp);
	quote = is_closed(*word, arg);
	ch = next_word(word);
	while (ch != '\'')
	{
		if (ch == '\0')
			break ;
		if (ch == '$' && !quote)
		{
			expansion_dollar(word, arg, args, shell);
			ch = now_word(word);
		}
		else
		{
			ft_charjoin(arg, ch);
			ch = next_word(word);
		}
	}
	next_word(word);
}

void	str(char **arg, char *ch, char **word)
{
	ft_charjoin(arg, *ch);
	*ch = next_word(word);
}
