/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:31:38 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 06:43:59 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static int	is_valid_command_token(t_token_type type)
{
	return (type == IS_WORD || type == IS_REDIRECT_IN || type == IS_REDIRECT_OUT
		|| type == IS_REDIRECT_APPEND || type == IS_REDIRECT_HEREDOC
		|| type == IS_OPEN_BRACKET);
}

static void	process_word_token(t_ast *node, t_token **tokens, size_t *argc)
{
	node->argv[*argc] = malloc(sizeof(t_expand_arg));
	node->argv[*argc]->value = ft_strdup((*tokens)->value);
	node->argv[*argc]->segments = (*tokens)->segments;
	(*argc)++;
	*tokens = (*tokens)->next;
}

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

static void	process_command_tokens(t_ast *node, t_token **tokens, size_t *argc)
{
	while (*tokens && is_valid_command_token((*tokens)->type))
	{
		if ((*tokens)->type == IS_WORD)
			process_word_token(node, tokens, argc);
		else
		{
			if (!process_redirection(node, tokens))
				return ;
		}
	}
	node->argv[*argc] = NULL;
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
