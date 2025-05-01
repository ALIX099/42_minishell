/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:58:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/01 20:14:26 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static unsigned int	count_tokens(char *line)
{
	unsigned int	i;

	//Checkpoint
}

t_token	*tokenize(char *line)
{
	unsigned int	tokens_count;
	t_token			*tokens;

	tokens = NULL;
	tokens_count = count_tokens(line);
	if (tokens_count == 0)
		return (NULL);
	tokens = malloc(sizeof(t_token) * (tokens_count + 1));
	if (!tokens)
		return (NULL);
	
}