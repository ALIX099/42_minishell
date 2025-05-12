/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:58:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/11 18:55:23 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static unsigned int	count_tokens(char *line)
{
	unsigned int	i;
	unsigned int	word_count;

	word_count = 0;
	i = 0;
	while (line && line[i])
	{
		while (line[i] && !ft_isspace(line[i]))
			i++;
		word_count++;
		while (line[i] && ft_isspace(line[i]))
			i++;
	}
	return (word_count);
}

static t_token_type	get_token_type(char *s)
{
	if (ft_strncmp(s, ">>", 2) == 0)
        return (IS_REDIRECT_APPEND);
    else if (ft_strncmp(s, "<<", 2) == 0)
        return (IS_REDIRECT_HEREDOC);
    else if (ft_strncmp(s, "|", 1) == 0)
        return (IS_PIPE);
    else if (ft_strncmp(s, "<", 1) == 0)
        return (IS_REDIRECT_IN);
    else if (ft_strncmp(s, ">", 1) == 0)
        return (IS_REDIRECT_OUT);
	else
        return (IS_WORD);
}
static char	*expand_envvars(char *s)
{
	//To do
}

static void	str_to_tokens(t_token *tokens, char *s)
{
	unsigned int	i;
	t_token			*new_token;

	new_token = malloc(sizeof(t_token));
	new_token->next = NULL;
	i = 0;
	if (s[i] == '\'')
	{
		tokens->type = IS_WORD;
		tokens->value = s;
	}
	else if (s[i] == '\"')
	{
		tokens->type = IS_WORD;
		tokens->value = expand_envvars(s);
	}
	else
	{
		while (s[i])
		{
			if (s[i] == '>' || s[i] == '<' || s[i] == '|')
			{
				
			}
		}
	}
}

static void	init_tokens(t_token *tokens, char **strs, unsigned int tokens_count)
{
    unsigned int	i;

    i = 0;
    while (i < tokens_count)
        str_to_tokens(tokens, strs[i++]);
}

t_token	*tokenize(char *line)
{
	unsigned int	tokens_count;
	char			**strs;
	t_token			*tokens;

	tokens = NULL;
	tokens_count = count_tokens(line);
	strs = ft_split_on_separator(line, " \v\f\r\t\n");
	if (!strs)
		return (NULL);
	init_tokens(tokens, strs, tokens_count);
	return (tokens);
}
