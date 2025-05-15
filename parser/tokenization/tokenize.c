/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:58:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/14 20:08:52 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_token	*tokenize(char *line)
{
	t_token			*tokens;

	tokens = NULL;
	init_tokens(tokens, line);
	return (tokens);
}
