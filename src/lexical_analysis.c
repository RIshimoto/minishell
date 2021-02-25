#include "../includes/minishell.h"

typedef enum u_token
{
	PIPE,		// |
	GT,			// >
	LT,			// < 
	DGT,		// >>
	SEMICOLON,	// ;
	IDENTIFY,	// String
	INPUT_END,	// End Of Input

	TOKEN_NUM	// don't care
} t_token;

//* To degug *//
char *command_name[TOKEN_NUM] = {
	"PIPE",		// |
	"GT",		// >
	"LT",		// < 
	"DGT",		// >>
	"SEMICOLON",// ;
	"IDENTIFY", // String
	"INPUT_END"	// End Of Input
};

typedef struct	s_ip
{
	t_token		sy;
	char		ch;
	char		*id_string;
}				t_ip;

bool equal(char *s, char *t)
{
	if (ft_strlen(s) != ft_strlen(t))
		return (false);
	else if (ft_strncmp(s, t, ft_strlen(s)) == 0)
		return (true);
	return (false);
}

char *ft_charjoin(char *old, char c)
{
	char *new;
	int len;
	int i;

	len = ft_strlen(old);
	new = (char *)malloc(sizeof(char) * (len + 2));
	i = -1;
	while (++i < len)
		new[i] = old[i];
	new[i++] = c;
	new[i] = '\0';
	free(old);
	return (new);
}

char next_ch(char *line)
{ char		ch;
	static int index;

	ch = line[index];
	if (line[index] != '\0')
		index++;
	return (ch);
}

void check_token2_sub(char *line, t_ip *ip)
{
	char *line2;

	if (ip->ch == '\0')
	{
		write(1, ">", 1);
		get_next_line(0, &line2);
		ft_strjoin(line, "\n");
		ft_strjoin(line, line2);
	}
	else
		ip->id_string = ft_charjoin(ip->id_string, ip->ch);
	ip->ch = next_ch(line);
}

void check_token2(char *line, t_ip *ip)
{
	if (ip->ch == '\"')
	{
		ip->ch = next_ch(line);
		while (ip->ch != '\"')
			check_token2_sub(line, ip);
		ip->ch = next_ch(line);
	}
	else if (ip->ch == '\'')
	{
		ip->ch = next_ch(line);
		while (ip->ch != '\'')
			check_token2_sub(line, ip);
		ip->ch = next_ch(line);
	}
	else if (ip->ch == '`')
	{
		ip->ch = next_ch(line);
		while (ip->ch != '`')
			check_token2_sub(line, ip);
		ip->ch = next_ch(line);
	}
	else if (ip->ch == '$')
	{
		ip->ch = next_ch(line);
		if (ip->ch == '{')
		{
			ip->ch = next_ch(line);
			while (ip->ch != '}')
				check_token2_sub(line, ip);
			ip->ch = next_ch(line);
		}
	}
	else if (ip->ch == '\\')
	{
		ip->ch = next_ch(line);
		ip->id_string = ft_charjoin(ip->id_string, ip->ch);
		ip->ch = next_ch(line);
	}
}

void check_token(char *line, t_ip *ip)
{
	if (ip->ch == '|')
	{
		ip->ch = next_ch(line);
		ip->sy = PIPE;
	}
	else if (ip->ch == '>')
	{
		if ((ip->ch = next_ch(line)) == '>')
		{
			ip->ch = next_ch(line);
			ip->sy = DGT;
		}
		else
			ip->sy = GT;
	}
	else if (ip->ch == '<')
	{
		ip->ch = next_ch(line);
		ip->sy = LT;
	}
	else if (ip->ch == ';')
	{
		ip->ch = next_ch(line);
		ip->sy = SEMICOLON;
	}
}

void get_token(char *line, t_ip *ip)
{
	free(ip->id_string);
	ip->id_string = ft_calloc(sizeof(char), 1);
	while (ip->ch == ' ' || ip->ch == '\t')
		ip->ch = next_ch(line);
	if (ip->ch == '\0')
		ip->sy = INPUT_END;
	else
	{
		if (ft_strchr("|><;", ip->ch) == NULL)
		{
			while (ft_strchr("|><; \0", ip->ch) == NULL)
			{
				ip->id_string = ft_charjoin(ip->id_string, ip->ch);
				ip->ch = next_ch(line);
				if (ft_strchr("\\\"\'", ip->ch) != NULL)
					check_token2(line, ip);
			}
			ip->sy = IDENTIFY;
		}
		else
		{
			ip->id_string = ft_charjoin(ip->id_string, ip->ch);
			check_token(line, ip);
		}
	}
	return ;
}


int main(int argc, char **argv)
{
	t_ip ip;

	ip.id_string = malloc(1);
	ip.ch = ' ';
	get_token(argv[1], &ip);
	//* To debug *//
	while (ip.sy != INPUT_END)
	{
		printf("[%s]:%s\n", ip.id_string,command_name[ip.sy]); 
		get_token(argv[1], &ip);
	}
	return 0;
}
