#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <signal.h>
#include <termios.h>
#include <termcap.h>
#include <ctype.h>
#include <fcntl.h>
#include "libcmds.h"
#include "shell_variable.h"

#define CTRLC	3
#define CTRLD	4
#define LF		10
#define ESC		27
#define DEL		127

enum u_token;
struct s_ip;
struct s_pos;
typedef enum u_token	t_token;
typedef struct s_ip		t_ip;
typedef struct s_pos	t_pos;
typedef t_list *		t_queue;
typedef char ***		t_args;

enum u_token
{
	PIPE,		// |
	REDIRECT,	// >>, >, <
	SEMICOLON,	// ;
	IDENTIFY,	// String
	INPUT_END,	// End Of Input

	TOKEN_NUM	// don't care
};

struct	s_ip
{
	t_token		sy;
	char		ch;
	char		*id_string;
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
};

struct  s_pos
{
	int cursor;
	int max_rg;
	int max_lf;
};

// prompt
void	prompt(char *ps, t_dlist **line);
void	term_mode(char *p);

// prompt_utils
void	insert(t_dlist **lst, char c, t_pos *pos);
void 	del(t_pos *pos, t_dlist **cursor);
void 	esc(t_pos *pos, t_dlist **cursor);
void	init_pos(t_pos *pos, char *ps);
void	ctrl_d(t_pos *pos, t_dlist **cursor);

// lexer
void lexer(t_dlist **line, t_queue *tokens, t_shell_var *sv);
char next_ch(t_dlist **line, t_ip *ip);
char *expand_parameter(t_dlist **line);
void quoting(t_dlist **line, t_ip *ip);
void metacharacter(t_dlist **line, t_ip *ip);

// parser
void parser(t_queue *tokens, t_shell_var *sv);

// exec
//void exec(t_args *args, t_shell_var *sv, t_queue *vars);
void exec(t_args args, t_queue *fds, t_shell_var *sv);
void assign_variable(t_queue *vars, t_shell_var *sv);
void redirect(t_queue *fds);
//char **fetch_path(t_args *args, t_shell_var *sv);
char **fetch_path(t_args args, t_shell_var *sv);

// queue
void push(char *str, t_queue *queue);
char *pop(t_queue *queue);

// error
void error(char *message, char *token);
void error2(char *message, char token);

#endif
