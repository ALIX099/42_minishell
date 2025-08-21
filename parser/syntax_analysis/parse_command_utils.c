/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:40:46 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 13:59:28 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

int	is_valid_command_token(t_token_type type)
{
	return (type == IS_WORD || type == IS_REDIRECT_IN || type == IS_REDIRECT_OUT
		|| type == IS_REDIRECT_APPEND || type == IS_REDIRECT_HEREDOC
		|| type == IS_OPEN_BRACKET);
}

t_segment	*deep_copy_segments(t_segment *source)
{
    t_segment	*head;
    t_segment	*curr_src;
    t_segment	*prev_dest;
	t_segment	*new_seg;

	head = NULL;
	curr_src = source;
	prev_dest = NULL;
    while (curr_src)
    {
        new_seg = malloc(sizeof(t_segment));
        if (!new_seg)
            return (NULL);
        new_seg->state = curr_src->state;
        new_seg->expandable = curr_src->expandable;
        if (curr_src->seg_txt)
            new_seg->seg_txt = ft_strdup(curr_src->seg_txt);
        else
            new_seg->seg_txt = NULL;
        new_seg->next = NULL;
        if (!head)
            head = new_seg;
        else
            prev_dest->next = new_seg;
        prev_dest = new_seg;
        curr_src = curr_src->next;
	}
    return (head);
}

void	process_word_token(t_ast *node, t_token **tokens, size_t *argc)
{
	node->argv[*argc] = malloc(sizeof(t_expand_arg));
	node->argv[*argc]->value = ft_strdup((*tokens)->value);
	node->argv[*argc]->segments = deep_copy_segments((*tokens)->segments);
	(*argc)++;
	*tokens = (*tokens)->next;
}

void	process_command_tokens(t_ast *node, t_token **tokens, size_t *argc)
{
	t_token	*curr_token;

	while (*tokens && is_valid_command_token((*tokens)->type))
	{
		curr_token = *tokens;
		if ((*tokens)->type == IS_WORD)
			process_word_token(node, tokens, argc), free_token(curr_token);
		else
		{
			if (!process_redirection(node, tokens))
				return ;
			free_token(curr_token);
		}
	}
	node->argv[*argc] = NULL;
}