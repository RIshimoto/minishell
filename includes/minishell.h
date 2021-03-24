#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <stdbool.h>
#include "get_next_line.h"
#include "libcmds.h"
#include "shell_variable.h"

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

typedef struct	s_ip
{
	t_token		sy;
	char		ch;
	char		*id_string;
	int			index;
	/* To degug 
	char *command_name[TOKEN_NUM] = {
		"PIPE",		// |
		"GT",		// >
		"LT",		// < 
		"DGT",		// >>
		"SEMICOLON",// ;
		"IDENTIFY", // String
		"INPUT_END"	// End Of Input
	};
	*/
}				t_ip;

typedef struct s_echo_flag
{
	int			n;
	int			another;
}				t_echo_flag;

typedef t_list * t_queue;
typedef char *** t_args;

// lexer
void lexer(char *line, t_queue *tokens, t_shell_var sv);
char next_ch(char *line, t_ip *ip);
char *expand_parameter(char **line, t_ip *ip, t_shell_var sv);
void quoting(char **line, t_ip *ip, t_shell_var sv);
void metacharacter(char **line, t_ip *ip);

// parser
void parser(t_queue *tokens, t_shell_var sv);
char **fetch_path(t_args *args, t_shell_var sv);
void go_exec_pipeline(t_args *args, t_shell_var sv);

void error(char *message, char *token);
void error2(char *message, char token);

#endif
