#include "../../includes/minishell.h"

static char	*check_ch(char ch, char ***word, char **key, t_shell *shell)
{
	if (ch == '?')
	{
		next_word(*word);
		free(*key);
		return (ft_itoa(shell->exit_status));
	}
	if (!ft_issnack_case(ch))
	{
		free(*key);
		return (ft_strdup("$"));
	}
	return (NULL);
}

char	*parameter(char **word, t_shell *shell)
{
	char	*key;
	char	*val;
	char	ch;
	char	*result;

	key = ft_calloc(sizeof(char), 1);
	ch = next_word(word);
	result = check_ch(ch, &word, &key, shell);
	if (result != NULL)
		return (result);
	while (ft_issnack_case(ch))
	{
		ft_charjoin(&key, ch);
		ch = next_word(word);
	}
	val = getenv(key);
	free(key);
	if (val == NULL)
		return (NULL);
	return (ft_strdup(val));
}
