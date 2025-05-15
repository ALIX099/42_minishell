/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:12:29 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/14 17:00:27 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_command	*parse(char *line)
{
	t_token 	*tokens;
	t_command	*cmds;

	cmds = NULL;
	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	cmds = parse_tokens(tokens);
	if (!cmds)
	{
		free(tokens);
		return (NULL);
	}
	return (cmds);
}