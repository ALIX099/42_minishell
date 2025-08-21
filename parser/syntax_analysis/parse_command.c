/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:31:38 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 13:50:49 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static t_ast	*init_command_node(t_token **tokens)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->argv = malloc(sizeof(t_expand_arg *) * (count_tok_args(*tokens) + 1));
	if (!node->argv)
		return (free(node), NULL);
	node->redirects = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*node;
	size_t	argc;

	if (!*tokens || !is_valid_command_token((*tokens)->type))
		return (syntax_error("unexpected token", *tokens), NULL);
	if ((*tokens)->type == IS_OPEN_BRACKET)
		return (create_subshell_node(tokens));
	node = init_command_node(tokens);
	if (!node)
		return (NULL);
	argc = 0;
	process_command_tokens(node, tokens, &argc);
	return (node);
}
