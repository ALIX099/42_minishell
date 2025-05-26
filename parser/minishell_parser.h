/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_parser.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:15:44 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/22 18:06:36 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHELL_PARSER_H
# define MINISHELL_PARSER_H

# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"

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
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;
void	init_tokens(t_token **tokens, char *s);
void	ft_tokenlist_add_back(t_token **token_list, t_token **new_token);
char	*extract_token(char *s, int *i_ptr);
t_token	*tokenize(char *line);

/*
	SYNTAX ANALYSIS
*/


/*
	REDIRECTIONS
*/
typedef enum e_redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC,
}	t_redirect_type;

typedef struct s_redirect
{
	t_redirect_type		type;
	char				*file;
	int					fd;
	struct s_redirect	*next;
}	t_redirect;


/*
	SEMANTIC ANALYSIS
*/
typedef struct s_command
{
	char				*command;
	char				**args;
	char				**envp;
	t_redirect			*redirects;
	struct s_command	*next;
}	t_command;

t_command	*parse(char *line);

//Utils
int		ft_isspace(char c);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strndup(char *s, int n);

#endif // MINISHELL_PARSER_H