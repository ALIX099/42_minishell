/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:20:30 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/21 23:15:33 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

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
	else
	return (IS_WORD);
}

static char	*expand_envvars(char *s)
{
	//To do
	(void)s;
	return (NULL);
}

static int	is_special_char(const char *s, int c)
{
	if (ft_strchr(s, c))
		return (1);
	return (0);
}

static void	str_to_tokens(t_token *tokens, char *s)
{
	t_token	*new_token;
	int		i;
	int		start;

	new_token = malloc(sizeof(t_token)); 
	ft_tokenlist_add_back(&tokens, new_token);
	i = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]))
			i++;
		start = i;
		while (s[i] && !is_special_char("<>'\"|$", s[i]))
			i++;
		if (is_special_char("()<>|$&", s[i]))
		{
			new_token->value = extract_token(s + i, &i);
			new_token->type = get_token_type(new_token->value);
		}
		else
		{
			new_token->value = ft_strndup(s + start, i - start + 1);
			new_token->type = get_token_type(new_token->value);
		}
		str_to_tokens(tokens, s + i);
	}
}

void	init_tokens(t_token *tokens, char *str)
{
	unsigned int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (!str[i])
		return;		
	str_to_tokens(tokens, str + i);
}
