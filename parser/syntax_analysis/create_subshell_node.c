/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_subshell_node.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:53:43 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/22 03:29:35 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_ast	*create_subshell_node(t_token **tokens)
{
	t_ast	*subshell;
	t_ast	*node;

	onto_next_token(tokens);
	subshell = parse_logical_expr(tokens);
	if (!*tokens || (*tokens)->type != IS_CLOSE_BRACKET)
	{
		syntax_error("expected closing parenthesis", *tokens);
		return (NULL);
	}
	onto_next_token(tokens);
	node = malloc(sizeof(t_ast));
	node->type = NODE_SUBSHELL;
	node->left = subshell;
	node->right = NULL;
	node->argv = NULL;
	node->redirects = NULL;
	return (node);
}
