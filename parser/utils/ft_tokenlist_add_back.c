/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenlist_add_back.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:21:41 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 17:24:47 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

void	ft_redirlist_add_back(t_redirect **redirs, t_redirect **new_redir)
{
	t_redirect	*tmp;
	
	if (!(*redirs))
	{
		*redirs = *new_redir;
		(*redirs)->next = NULL;
	}
	else
	{
		tmp = *redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = (*new_redir);
		(*new_redir)->next = NULL;
	}
}

void	ft_tokenlist_add_back(t_token **tokens, t_token **new_token)
{
	t_token	*tmp;
	
	if (!(*tokens))
	{
		*tokens = *new_token;
		(*tokens)->next = NULL;
	}
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = (*new_token);
		(*new_token)->next = NULL;
	}
}