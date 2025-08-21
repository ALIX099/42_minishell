/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:51:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 22:41:27 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_ast	*parse(char *line)
{
	t_ast	*ast;
	t_token	*tokens;

	ast = NULL;
	tokens = tokenize(&line);
	if (!tokens)
		return (NULL);
	ast = syntactic_analysis(tokens);
	if (!ast)
		return (NULL);
	return (ast);
}
