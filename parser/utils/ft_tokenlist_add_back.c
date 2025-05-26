/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenlist_add_back.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:21:41 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/22 18:06:54 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

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