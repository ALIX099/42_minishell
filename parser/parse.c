/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 20:12:29 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/01 20:13:58 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_command	*parse(char *line)
{
	t_token 	*tokens;
	t_command	*cmds;

	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	//TO DO
}