#include "../../../includes/minishell.h"

static void dollar(t_dlist **line, t_ip *ip)
{
	ft_charjoin(&ip->id_string, ip->ch);
	next_ch(line, ip);
	if (ip->ch == '{')
	{
		while (next_ch(line, ip) != '}')
		{
			if (ip->ch == '\0')
				break;
			ft_charjoin(&ip->id_string, ip->ch);
		}
	}
	else
	{
		while (ft_issnack_case(ip->ch)) 
		{
			ft_charjoin(&ip->id_string, ip->ch);
			next_ch(line, ip);
		}
	}
}

static void numeric(t_dlist **line, t_ip *ip, t_list **tokens)
{
	while (ft_isdigit(ip->ch))
	{
		ft_charjoin(&ip->id_string, ip->ch);
		next_ch(line, ip);
	}
	if (ip->ch == '>' || ip->ch == '<')
		metacharacter(line, ip, tokens);
}

static void string(t_dlist **line, t_ip *ip)
{
	ft_charjoin(&ip->id_string, ip->ch);
	next_ch(line, ip);
}

void literal(t_dlist **line, t_ip *ip, t_list **tokens)
{
	ip->sy = IDENTIFY;
	while (ft_strchr("|><;& \t\0", ip->ch) == NULL)
	{
		if (ft_isdigit(ip->ch))
			numeric(line, ip, tokens);
		else if (ip->ch == '$')
			dollar(line, ip);
		else if (ft_strchr("\"\'\\", ip->ch) != NULL)
			quoting(line, ip);
		else
			string(line, ip);
	}
	if (ft_strchr(ip->id_string, '*') != NULL)
		wildcard(line, ip, tokens);
}
