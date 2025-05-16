/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:20:30 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/16 22:26:46 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static t_token_type	get_token_type(char *s)
{
	if (ft_strcmp(s, ">>", 2) == 0)
		return (IS_REDIRECT_APPEND);
	else if (ft_strcmp(s, "<<", 2) == 0)
		return (IS_REDIRECT_HEREDOC);
	else if (ft_strcmp(s, "|", 1) == 0)
		return (IS_PIPE);
	else if (ft_strcmp(s, "<", 1) == 0)
		return (IS_REDIRECT_IN);
	else if (ft_strcmp(s, ">", 1) == 0)
		return (IS_REDIRECT_OUT);
	else
		return (IS_WORD);
}

static char	*expand_envvars(char *s)
{
	//To do
}

static void	separate_tokens(char *s, t_token *new_token)
{
	unsigned int	i;
	unsigned int	op_len;
	t_token			*next_token;

	i = 0;
	op_len = 1; 
	while (s[i] && s[i] != '>' && s[i] != '<' && s[i] != '|')
		i++;
	if (i > 0)
	{
		new_token->type = IS_WORD;
		new_token->value = ft_strndup(s, i);
		if (s[i])
		{
			next_token = malloc(sizeof(t_token));
			if (!next_token)
				return ;
			next_token->next = NULL;
			new_token->next = next_token;
			separate_tokens(s + i, next_token);
		}
		else
			new_token->next = NULL;
		return ;
	}
	if (s[i])
	{
		if ((s[i] == '>' && s[i + 1] == '>') || (s[i] == '<' && s[i + 1] == '<'))
			op_len = 2;
		new_token->value = ft_strndup(s + i, op_len);
		new_token->type = get_token_type(new_token->value);
		new_token->next = NULL;
	}
	if (s[i + op_len])
	{
		next_token = malloc(sizeof(t_token));
		if (!next_token)
			return ;
		next_token->next = NULL;
		new_token->next = next_token;
		separate_tokens(s + i, next_token);
	}
	else
		new_token->next = NULL;
}

static void	str_to_tokens(t_token *tokens, char *s)
{
	unsigned int	i;
	t_token			*tmp;
	t_token			*new_token;

	if (!tokens)
	{
		tokens = malloc(sizeof(t_token));
		if (!tokens)
			return ;
		new_token = tokens;
		new_token->next = NULL;
	}
	else
	{
		new_token = malloc(sizeof(t_token));
		if (!new_token)
			return ;
		new_token->next = NULL;
		tmp = tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
	i = 0;
	if (s[i] == '\'')
	{
		new_token->type = IS_WORD;
		new_token->value = s;
	}
	else if (s[i] == '\"')
	{
		tokens->type = IS_WORD;
		tokens->value = expand_envvars(s);
	}
	else
		separate_tokens(s, new_token);
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