/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:46:00 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/16 22:50:49 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

char	*ft_strndup(char *s, int n)
{
	size_t	i;
	char	*new_s;
	size_t	s_len;

	if (!s1 || n <= 0)
		return (NULL);
	i = 0;
	s_len = ft_strlen(s1);
	if (n > s_len)
		n = s_len;
	new_s = (char *)malloc(n + 1);
	if (!new_s)
		return (NULL);
	while (i < n)
	{
		new_s[i] = s1[i];
		i++;
	}
	new_s[i] = '\0';
	return (new_s);
}
