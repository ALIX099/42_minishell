/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 17:26:12 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/12 17:57:41 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

void	syntax_error(const char *msg, t_token *token)
{
	if (token)
		fprintf(stderr, "rsh: syntax error near '%s': %s\n", msg, token->value);
	else
		fprintf(stderr, "rsh: syntax error near '%s'\n", msg);
}
