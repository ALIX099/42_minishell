/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_subshell_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:53:43 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 06:45:36 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_ast	*create_subshell_node(t_token **tokens)
{
	t_ast	*subshell;
	t_ast	*node;

	*tokens = (*tokens)->next;
	subshell = parse_logical_expr(tokens);
	if (!*tokens || (*tokens)->type != IS_CLOSE_BRACKET)
	{
		syntax_error("expected closing parenthesis", *tokens);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	node = malloc(sizeof(t_ast));
	node->type = NODE_SUBSHELL;
	node->left = subshell;
	node->right = NULL;
	node->argv = NULL;
	node->redirects = NULL;
	return (node);
}
