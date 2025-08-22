/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 03:51:41 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/22 03:29:54 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

int	process_redirection(t_ast *node, t_token **tokens)
{
	t_redirect	*redir;

	if ((*tokens)->type == IS_REDIRECT_HEREDOC)
		redir = setup_heredoc(tokens);
	else
		redir = setup_redirection(tokens, get_redirect_type((*tokens)->type));
	if (!redir)
		return (0);
	ft_redirlist_add_back(&(node->redirects), &redir);
	onto_next_token(tokens);
	return (1);
}

t_redirect	*setup_redirection(t_token **tokens, t_redirect_type type)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	redir->type = type;
	onto_next_token(tokens);
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
