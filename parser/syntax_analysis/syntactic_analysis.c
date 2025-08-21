/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntactic_analysis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:36:27 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 13:52:23 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_redirect_type	get_redirect_type(t_token_type tok_type)
{
	if (tok_type == IS_REDIRECT_IN)
		return (REDIRECT_IN);
	if (tok_type == IS_REDIRECT_OUT)
		return (REDIRECT_OUT);
	if (tok_type == IS_REDIRECT_APPEND)
		return (REDIRECT_APPEND);
	if (tok_type == IS_REDIRECT_HEREDOC)
		return (REDIRECT_HEREDOC);
	return (-1);
}

size_t	count_tok_args(t_token *tokens)
{
	size_t	ret;
	t_token	*tmp;

	ret = 0;
	tmp = tokens;
	while (tmp && (tmp->type == IS_WORD || tmp->type == IS_REDIRECT_IN
			|| tmp->type == IS_REDIRECT_OUT || tmp->type == IS_REDIRECT_APPEND
			|| tmp->type == IS_REDIRECT_HEREDOC))
	{
		if (tmp->type == IS_WORD)
			++ret;
		else if (tmp->next)
			tmp = tmp->next;
		tmp = tmp->next;
	}
	return (ret);
}

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*parent;

	left = parse_command(tokens);
	while (*tokens && (*tokens)->type == IS_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_command(tokens);
		if (!right)
			return (syntax_error("expected command after operator", *tokens),
				NULL);
		parent = malloc(sizeof(t_ast));
		parent->type = NODE_PIPE;
		parent->right = right;
		parent->argv = NULL;
		parent->left = left;
		parent->redirects = NULL;
		left = parent;
	}
	return (left);
}

t_ast	*parse_logical_expr(t_token **tokens)
{
	t_ast		*left;
	t_ast_type	type;
	t_ast		*right;
	t_ast		*parent;

	left = parse_pipeline(tokens);
	while (*tokens && ((*tokens)->type == IS_AND || (*tokens)->type == IS_OR))
	{
		if ((*tokens)->type == IS_AND)
			type = NODE_AND;
		else
			type = NODE_OR;
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		if (!right)
			return (syntax_error("no command after logical op",*tokens), NULL);
		parent = malloc(sizeof(t_ast));
		parent->type = type;
		parent->left = left;
		parent->right = right;
		parent->argv = NULL;
		parent->redirects = NULL;
		left = parent;
	}
	return (left);
}

t_ast	*syntactic_analysis(t_token *tokens)
{
	t_ast	*ast;

	ast = parse_logical_expr(&tokens);
	if (!ast)
		return (NULL);
	if (tokens)
	{
		syntax_error("unexpected token at end of input", tokens);
		return (NULL);
	}
	return (ast);
}
