/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_subshell_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:53:43 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 23:42:17 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_ast	*create_subshell_node(t_token **tokens)
{
	t_ast	*subshell;
	t_ast	*node;
	t_token	*old;

	old = *tokens;
	*tokens = (*tokens)->next;
	free_token(old);
	subshell = parse_logical_expr(tokens);
	if (!*tokens || (*tokens)->type != IS_CLOSE_BRACKET)
	{
		syntax_error("expected closing parenthesis", *tokens);
		return (NULL);
	}
	old = *tokens;
	*tokens = (*tokens)->next;
	free_token(old);
	node = malloc(sizeof(t_ast));
	node->type = NODE_SUBSHELL;
	node->left = subshell;
	node->right = NULL;
	node->argv = NULL;
	node->redirects = NULL;
	return (node);
}
