/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:58:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 06:50:50 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_token_type	get_token_type(char *s)
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
	else if (ft_strcmp(s, "||") == 0)
		return (IS_OR);
	else if (ft_strcmp(s, "&&") == 0)
		return (IS_AND);
	else if (ft_strcmp(s, "(") == 0)
		return (IS_OPEN_BRACKET);
	else if (ft_strcmp(s, ")") == 0)
		return (IS_CLOSE_BRACKET);
	else
		return (IS_WORD);
}

int	is_special_char(const char *s, int c)
{
	if (ft_strchr(s, c))
		return (1);
	return (0);
}

t_token	*tokenize(char **line)
{
	t_token	*tokens;
	char	*trimmed_line;

	tokens = NULL;
	trimmed_line = ft_strtrim(*line, " \n\t\v\r\f");
	line = &trimmed_line;
	init_tokens(&tokens, *line);
	return (tokens);
}
