/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 18:58:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/12 17:54:07 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

t_token	*tokenize(char **line)
{
	t_token	*tokens;
	char	*trimmed_line;

	tokens = NULL;
	trimmed_line = ft_strtrim(*line, " \n\t\v\r\f");
	free(*line);
	line = &trimmed_line;
	init_tokens(&tokens, *line);
	return (tokens);
}
