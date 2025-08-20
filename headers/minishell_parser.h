/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:15:44 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 06:27:40 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSER_H
# define MINISHELL_PARSER_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <dirent.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <stddef.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/*
	EXPANSION
*/
typedef enum e_segment_state
{
	LITERAL,
	S_QUOTED,
	D_QUOTED
}						t_segment_state;

typedef enum e_expand_state
{
	EXPAND,
	NO_EXPAND
}						t_expand_state;


typedef struct s_segment
{
	char				*seg_txt;
	t_segment_state		state;
	t_expand_state		expandable;
	struct s_segment	*next;
}						t_segment;

typedef struct e_expand_arg
{
	char				*value;
	t_segment			*segments;
}						t_expand_arg;

/*
	TOKENIZATION
*/
typedef enum e_token_type
{
	IS_WORD,
	IS_PIPE,
	IS_REDIRECT_IN,
	IS_REDIRECT_OUT,
	IS_REDIRECT_APPEND,
	IS_REDIRECT_HEREDOC,
	IS_OPEN_BRACKET,
	IS_CLOSE_BRACKET,
	IS_AND,
	IS_OR,
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	t_segment			*segments;
	t_expand_state		expandable;
	struct s_token		*next;
}						t_token;
void	init_tokens(t_token **tokens, char *s);
void	ft_tokenlist_add_back(t_token **token_list, t_token **new_token);
char	*extract_token(char *s, int *i_ptr);
t_token	*tokenize(char **line);
/*
REDIRECTIONS
*/
typedef enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
}						t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	t_expand_arg		*file;
	int					fd;
	struct s_redirect	*next;
}						t_redirect;

void	ft_redirlist_add_back(t_redirect **redirs, t_redirect **new_redir);
/*
	SYNTAX ANALYSIS
*/
typedef enum e_ast_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_SUBSHELL
}						t_ast_type;

typedef struct s_ast
{
	t_ast_type			type;
	struct s_ast		*left;
	struct s_ast		*right;
	t_expand_arg		**argv;
	t_redirect			*redirects;
	struct s_exec		*exec;
}						t_ast;

t_ast					*syntactic_analysis(t_token *tokens);
t_ast					*parse(char *line);
// Recursive Decsent Parser
t_ast					*parse_logical_expr(t_token **tokens);
t_ast					*parse_pipeline(t_token **tokens);
t_ast					*parse_command(t_token **tokens);
// Syntax Error Reporting
void					syntax_error(const char *msg, t_token *token);

/* REPLACED WITH AST

typedef struct s_command
{
	char				*command;
	char				**args;
	char				**envp;
	t_redirect			*redirects;
	struct s_command	*next;
}						t_command;
*/

// Utils
int						ft_isspace(char c);
int						ft_strcmp(const char *s1, const char *s2);
char					*ft_strndup(char *s, int n);

#endif // MINISHELL_PARSER_H