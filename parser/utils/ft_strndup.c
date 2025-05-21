/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:46:00 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/20 16:06:06 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

char	*ft_strndup(char *s, int n)
{
	int		i;
	char	*new_s;
	int		s_len;

	if (!s || n <= 0)
		return (NULL);
	i = 0;
	s_len = ft_strlen(s);
	if (n > s_len)
		n = s_len;
	new_s = (char *)malloc(n + 1);
	if (!new_s)
		return (NULL);
	while (i < n)
	{
		new_s[i] = s[i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}
