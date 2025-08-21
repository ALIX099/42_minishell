/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:15:44 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 23:08:17 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_PARSER_H
# define MINISHELL_PARSER_H

# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
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
int						is_special_char(const char *s, int c);
t_token_type			get_token_type(char *s);
void					init_tokens(t_token **tokens, char *s);
void					ft_tokenlist_add_back(t_token **token_list,
							t_token **new_token);
char					*extract_token(char *s, int *i_ptr);
t_token					*tokenize(char **line);
// Segmentation utils
t_segment				*create_seg(char *text, t_segment_state type);
void					add_seg(t_segment **head, t_segment *new_segment);
int						read_until_quote(char **s, int i, char quote,
							char **out);
int						process_token_segments(t_token *token, char *s, int i);
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

typedef struct s_heredoc
{
	int					fd;
	char				*delimeter;
	int					quoted;
	char				*raw_body;
}						t_heredoc;

typedef struct s_redirect
{
	t_redirect_type		type;
	t_expand_arg		*file;
	int					fd;
	t_heredoc			*heredoc;
	struct s_redirect	*next;
}						t_redirect;

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

// Segmetation
int						process_quoted_segment(t_token *token, char *s, int i,
							char quote);
t_segment				*deep_copy_segments(t_segment *source);
void					process_command_tokens(t_ast *node, t_token **tokens, size_t *argc);
void					process_word_token(t_ast *node, t_token **tokens, size_t *argc);
int						is_valid_command_token(t_token_type type);
t_ast					*syntactic_analysis(t_token *tokens);
t_ast					*parse(char *line);
size_t					count_tok_args(t_token *tokens);
// Recursive Decsent Parser
t_ast					*parse_logical_expr(t_token **tokens);
t_ast					*parse_pipeline(t_token **tokens);
t_ast					*parse_command(t_token **tokens);
t_ast					*create_subshell_node(t_token **tokens);
// Syntax Error Reporting
void					syntax_error(const char *msg, t_token *token);
// Redirections utils
t_redirect_type			get_redirect_type(t_token_type tok_type);
void					ft_redirlist_add_back(t_redirect **redir_list,
							t_redirect **new_redir);
t_redirect				*setup_heredoc(t_token **tokens);
int						process_redirection(t_ast *node, t_token **tokens);
t_redirect				*setup_redirection(t_token **tokens,
							t_redirect_type type);
// Utils
int						ft_isspace(char c);
int						ft_strcmp(const char *s1, const char *s2);
char					*ft_strndup(char *s, int n);
// Cleanup
void					free_ast(t_ast *ast);
void					free_token(t_token *token);
void 					free_token_list(t_token *tokens);
#endif // MINISHELL_PARSER_H