/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token_segments.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:25:00 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/22 03:02:22 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static int	process_plain_segment(t_token *token, char *s, int i, int start)
{
	int		plain_start;
	char	*plain_text;

	plain_start = i;
	while (s[i] && !ft_isspace(s[i]) && ((!is_special_char("()<>|&", s[i])
				|| s[start] == '\'' || s[start] == '"')) && s[i] != '\''
		&& s[i] != '"')
		i++;
	plain_text = ft_strndup(s + plain_start, i - plain_start);
	add_seg(&token->segments, create_seg(plain_text, LITERAL));
	return (i);
}

static void	build_token_value(t_token *token)
{
	t_segment	*seg;
	char		*temp;

	token->value = ft_strdup("");
	seg = token->segments;
	while (seg)
	{
		temp = token->value;
		token->value = ft_strjoin(token->value, seg->seg_txt);
		free(temp);
		seg = seg->next;
	}
	token->type = IS_WORD;
}

static int	handle_segment_char(t_token *token, char *s, int i, int start)
{
	char	quote_char;

	if (s[i] == '\'' || s[i] == '"')
	{
		quote_char = s[i];
		i = process_quoted_segment(token, s, i, quote_char);
		if (i < 0)
			return (-1);
		if (s[i] && is_special_char("()<>|&", s[i]))
			return (i);
	}
	else
	{
		if (is_special_char("()<>|&", s[i]))
			return (i);
		i = process_plain_segment(token, s, i, start);
	}
	return (i);
}

int	process_token_segments(t_token *token, char *s, int i)
{
	int	start;
	int	result;

	start = i;
	token->segments = NULL;
	token->expandable = EXPAND;
	while (s[i] && !ft_isspace(s[i]) && (!is_special_char("()<>|&", s[i])
			|| s[start] == '\'' || s[start] == '"'))
	{
		result = handle_segment_char(token, s, i, start);
		if (result < 0)
			return (-1);
		if (result == i)
			break ;
		i = result;
	}
	build_token_value(token);
	return (i - start);
}
