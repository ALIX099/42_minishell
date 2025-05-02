/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_on_separator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:08:50 by ikarouat          #+#    #+#             */
/*   Updated: 2025/05/02 15:15:00 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static void	free_chunks(char **strs, unsigned int exist)
{
	unsigned int	i;

	i = 0;
	while (i < exist)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

static unsigned int	word_count(const char *s, char *sep)
{
	unsigned int	count_w;

	count_w = 0;
	while (*s)
	{
		if (ft_strchr(sep, *s))
			s++;
		else
		{
			count_w++;
			while (ft_strchr(sep, *s) != NULL && *s != '\0')
				s++;
		}
	}
	return (count_w);
}

static unsigned int	next_delimiter(const char *s, char *sep)
{
	unsigned int	count;

	count = 0;
	while (ft_strchr(sep, *(s + count)) == NULL && *(s + count) != '\0')
		count++;
	return (count);
}

static char	*get_next_word(char **strs, const char **s
	, char *sep, unsigned int exist)
{
	char			*chunk;
	unsigned int	del;
	unsigned int	i;

	i = 0;
	while (ft_strchr(sep, **s))
		(*s)++;
	del = next_delimiter(*s, sep);
	chunk = (char *)malloc(del + 1);
	if (!chunk)
	{
		free_chunks(strs, exist);
		return (NULL);
	}
	while (i < del)
		chunk[i++] = *((*s)++);
	chunk[i] = '\0';
	return (chunk);
}

char	**ft_split_on_separator(char const *s, char *sep)
{
	char			**res;
	char			*chunk;
	unsigned int	i;
	unsigned int	res_len;

	if (!s)
		return (NULL);
	res_len = word_count(s, sep);
	res = (char **)malloc((res_len + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (*s && i < res_len)
	{
		chunk = get_next_word(res, &s, sep, i);
		if (!chunk)
			return (NULL);
		else
			res[i++] = chunk;
	}
	res[res_len] = NULL;
	return (res);
}
