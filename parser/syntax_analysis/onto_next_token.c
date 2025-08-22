/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   onto_next_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:57:07 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 03:07:42 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

void	onto_next_token(t_token **tokens)
{
	t_token		*old;

	old = *tokens;
	*tokens = (*tokens)->next;
	free_token(old);
}
