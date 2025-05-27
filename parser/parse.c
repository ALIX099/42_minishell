/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:12:29 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/27 17:12:50 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_command	*parse(char *line)
{
	t_token 	*tokens;
	t_command	*cmds;

	cmds = NULL;
	//generate lexemes
	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	while (tokens != NULL)
		(printf("<%s , %i>\n", tokens->value, tokens->type), tokens = tokens->next);
	//validate conformance to grammar
	//cmds = syntactic_analysis(tokens);
	if (!cmds)
	{
		free(tokens);
		return (NULL);
	}
	return (cmds);
}