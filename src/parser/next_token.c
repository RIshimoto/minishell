#include "../../includes/minishell.h"

void	next_token(t_ip *ip, t_list **tokens)
{
	if (ip->sy == ERR)
		return ;
	*tokens = (*tokens)->next;
	*ip = *(t_ip *)(*tokens)->content;
}
