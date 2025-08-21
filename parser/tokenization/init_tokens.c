/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:20:30 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 23:43:24 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static t_token	*create_new_token(t_token **tokens)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	ft_tokenlist_add_back(tokens, &new_token);
	return (new_token);
}

static int	handle_token_type(t_token *token, char *s, int start)
{
	int	result;
	int	i;

	if (s[start] == '\'' || s[start] == '"')
	{
		result = process_token_segments(token, s, start);
		if (result < 0)
			return (-1);
		return (start + result);
	}
	else if (is_special_char("()<>|&", s[start]))
	{
		i = 0;
		token->value = extract_token(s + start, &i);
		token->type = get_token_type(token->value);
		add_seg(&token->segments, create_seg(ft_strdup(token->value), LITERAL));
		return (start + i);
	}
	else
	{
		result = process_token_segments(token, s, start);
		if (result < 0)
			return (-1);
		return (start + result);
	}
}

static int	skip_whitespace(char *s)
{
	int	i;

	i = 0;
	while (s[i] && ft_isspace(s[i]))
		i++;
	return (i);
}

static void	str_to_tokens(t_token **tokens, char *s)
{
	t_token	*new_token;
	int		start;
	int		next_pos;
	t_token	*head;

	if (!s || !*s)
		return ;
	head = *tokens;
	start = skip_whitespace(s);
	if (!s[start])
		return ;
	new_token = create_new_token(tokens);
	if (!new_token)
		return ;
	new_token->segments = NULL;
	next_pos = handle_token_type(new_token, s, start);
	if (next_pos < 0)
	{
		free_token_list(head);
		*tokens = NULL;
		return ;
	}
	str_to_tokens(tokens, s + next_pos);
}

void	init_tokens(t_token **tokens, char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (!str[i])
		return ;
	str_to_tokens(tokens, str + i);
}
