/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:58:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/02 15:49:41 by ikarouat         ###   ########.fr       */
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
		while (!ft_isspace(line[i]))
			i++;
		word_count++;
		while (ft_isspace(line[i]))
			i++;
	}
	return (word_count);
}

static t_token_type	get_token_type(char *s)
{
	if (ft_strcmp(s, ">>") == 0)
        return (IS_REDIRECT_APPEND);
    else if (ft_strcmp(s, "<<") == 0)
        return (IS_REDIRECT_HEREDOC);
    else if (ft_strcmp(s, "|") == 0)
        return (IS_PIPE);
    else if (ft_strcmp(s, "<") == 0)
        return (IS_REDIRECT_IN);
    else if (ft_strcmp(s, ">") == 0)
        return (IS_REDIRECT_OUT);
    else if (ft_strcmp(s, "$") == 0)
        return (IS_DOLLAR);
    else if (ft_strcmp(s, "'") == 0)
        return (IS_QUOTE);
    else if (ft_strcmp(s, "\"") == 0)
        return (IS_DQUOTE);
    else
        return (IS_WORD);	
}

static void	init_tokens(t_token *tokens, char **strs, unsigned int tokens_count)
{
    unsigned int	i;

    i = 0;
    while (i < tokens_count)
    {
        tokens[i].type = get_token_type(strs[i]);
        tokens[i].value = strs[i];
        i++;
    }
    tokens[tokens_count].value = NULL;
}

t_token	*tokenize(char *line)
{
	unsigned int	tokens_count;
	char			**strs;
	t_token			*tokens;

	tokens = NULL;
	tokens_count = count_tokens(line);
	strs = ft_split_on_separator(line, " \v\f\r\t\n");
	if (tokens_count == 0)
		return (NULL);
	tokens = malloc(sizeof(t_token) * (tokens_count + 1));
	if (!tokens)
		return (NULL);
	init_tokens(tokens, strs, tokens_count);
	free_chunks(strs, tokens_count);
	return (tokens);
}
