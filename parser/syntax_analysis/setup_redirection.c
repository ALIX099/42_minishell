/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:51:41 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 23:17:21 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

int	process_redirection(t_ast *node, t_token **tokens)
{
	t_redirect	*redir;
	t_token		*old;

	if ((*tokens)->type == IS_REDIRECT_HEREDOC)
		redir = setup_heredoc(tokens);
	else
		redir = setup_redirection(tokens, get_redirect_type((*tokens)->type));
	if (!redir)
		return (0);
	ft_redirlist_add_back(&(node->redirects), &redir);
	old = *tokens;
	*tokens = (*tokens)->next;
	free_token(old);
	return (1);
}

t_redirect	*setup_redirection(t_token **tokens, t_redirect_type type)
{
	t_redirect	*redir;
	t_token		*old;

	redir = malloc(sizeof(t_redirect));
	redir->type = type;
	old = *tokens;
	*tokens = (*tokens)->next;
	free_token(old);
	if (!*tokens || (*tokens)->type != IS_WORD)
	{
		syntax_error("expected filename after redirection", *tokens);
		free(redir);
		return (NULL);
	}
	redir->file = malloc(sizeof(t_expand_arg));
	redir->file->value = ft_strdup((*tokens)->value);
	redir->file->segments = deep_copy_segments((*tokens)->segments);
	redir->heredoc = NULL;
	return (redir);
}
