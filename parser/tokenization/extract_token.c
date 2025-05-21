/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 23:00:28 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/21 23:53:42 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

char	*extract_token(char *s, int *i_ptr)
{
	int		op_len;
	char	*tok_val;

	op_len = 1;
	if (*s == "&" && *(s + 1) == *s)
		++op_len;
	else if (ft_strchr("<>|", *s))
	{
		if (*(s + 1) == *s)
			++op_len;
	}
	tok_val = ft_strndup(s, op_len);
	*i_ptr = *i_ptr + op_len;
}
