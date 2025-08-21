/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word_splitting.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:09:22 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 07:02:42 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	had_variable_expansion(t_segment *seg)
{
	char	*dollar;

	if (seg->state == LITERAL || seg->state == D_QUOTED)
	{
		dollar = ft_strchr(seg->seg_txt, '$');
		if (dollar && (ft_isalnum(*(dollar + 1)) || *(dollar + 1) == '_'))
			return (1);
	}
	return (0);
}

static int	should_split_arg(char *value, t_segment *segments)
{
	t_segment	*seg;

	if (!ft_strchr(value, ' '))
		return (0);
	seg = segments;
	while (seg)
	{
		if (seg->state == LITERAL && ft_strchr(seg->seg_txt, '*'))
			return (1);
		if (seg->state == LITERAL && had_variable_expansion(seg))
			return (1);
		seg = seg->next;
	}
	return (0);
}

static int	count_split_words(char **split_words)
{
	int	count;

	count = 0;
	while (split_words[count])
		count++;
	return (count);
}

void	handle_word_splitting(t_ast *cmd_node)
{
	int		i;
	int		word_count;
	char	**split_words;

	if (!cmd_node || !cmd_node->argv)
		return ;
	i = 0;
	while (cmd_node->argv[i])
	{
		if (!should_split_arg(cmd_node->argv[i]->value,
				cmd_node->argv[i]->segments))
		{
			i++;
			continue ;
		}
		split_words = ft_split(cmd_node->argv[i]->value, ' ');
		word_count = count_split_words(split_words);
		if (word_count <= 1)
		{
			i++;
			continue ;
		}
		i = process_split(cmd_node, i, split_words, word_count);
	}
}
