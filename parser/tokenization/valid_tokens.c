/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:48:27 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/10 12:47:07 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

int	valid_tokens(t_token *tokens)
{
	int	i;
	int	is_first_word;

	i = 0;
	if (tokens[i].type != IS_WORD && tokens[i].type != IS_REDIRECT_HEREDOC)
	{
		printf("Error: %s Invalid command\n", tokens[i].value);
		return (0);
	}
	while (tokens && tokens[i].value != NULL)
	{
			
	}
	return (1);
}
