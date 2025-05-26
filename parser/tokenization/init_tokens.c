/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 17:20:30 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/26 22:23:49 by ikarouat         ###   ########.fr       */
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
	else if (ft_strcmp(s, "||") == 0)
		return (IS_OR);
	else if (ft_strcmp(s, "&&") == 0)
		return (IS_AND);
	else if (ft_strcmp(s, "(") == 0)
		return (IS_OPEN_BRACKET);
	else if (ft_strcmp(s, ")"))
		return (IS_CLOSE_BRACKET);
	else
		return (IS_WORD);
}

//static char	*expand_envvars(char *s)
//{
//	//To do
//	(void)s;
//	return (NULL);
//}

static int	is_special_char(const char *s, int c)
{
	if (ft_strchr(s, c))
		return (1);
	return (0);
}

static void	str_to_tokens(t_token **tokens, char *s)
{
	t_token	*new_token;
	int		i;
	int		start;

	new_token = malloc(sizeof(t_token)); 
	ft_tokenlist_add_back(tokens, &new_token);
	i = 0;
	while (s[i] && ft_strlen(s) != 0)
	{
		while (ft_isspace(s[i]))
			i++;
		start = i;
		while (s[i] && !is_special_char("()<>|$&", s[i]) && !ft_isspace(s[i]))
			i++;
		if (s[i] && is_special_char("()<>|$&", s[i]))
		{
			//TO ADD:check if the previous char is not a space and add the word from start if that's the case
			if (i != 0 && !ft_isspace(s[i - 1]))
			{
				new_token->value = ft_strndup(s + start, i - start + 1);
				new_token->type = get_token_type(new_token->value);
				str_to_tokens(tokens, s + i);
			}
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

void	init_tokens(t_token **tokens, char *str)
{
	unsigned int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (!str[i])
		return;		
	str_to_tokens(tokens, str + i);
}
