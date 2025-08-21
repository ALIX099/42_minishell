/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   segmentation_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:06:28 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 06:50:37 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_segment	*create_seg(char *text, t_segment_state type)
{
	t_segment	*segment;

	segment = malloc(sizeof(t_segment));
	if (!segment)
		return (NULL);
	segment->seg_txt = text;
	segment->state = type;
	if (type == S_QUOTED)
		segment->expandable = NO_EXPAND;
	else
		segment->expandable = EXPAND;
	segment->next = NULL;
	return (segment);
}

void	add_seg(t_segment **head, t_segment *new_segment)
{
	t_segment	*current;

	if (!*head)
		*head = new_segment;
	else
	{
		current = *head;
		while (current->next)
			current = current->next;
		current->next = new_segment;
	}
}

int	read_until_quote(char **s, int i, char quote, char **out)
{
	int	start;
	int	found;

	start = i + 1;
	found = 0;
	while ((*s)[++i])
	{
		if ((*s)[i] == quote)
		{
			found = 1;
			break ;
		}
	}
	if (!found)
	{
		write(2, "rsh: Unclosed quote\n", 20);
		*out = NULL;
		return (-1);
	}
	*out = ft_strndup(*s + start, i - start);
	return (i + 1);
}

int	process_quoted_segment(t_token *token, char *s, int i, char quote)
{
	char			*quoted_content;
	int				next_pos;
	t_segment_state	seg_type;

	next_pos = read_until_quote(&s, i, quote, &quoted_content);
	if (next_pos < 0)
		return (-1);
	if (quote == '\'')
	{
		seg_type = S_QUOTED;
		token->expandable = NO_EXPAND;
	}
	else
		seg_type = D_QUOTED;
	add_seg(&token->segments, create_seg(quoted_content, seg_type));
	return (next_pos);
}
