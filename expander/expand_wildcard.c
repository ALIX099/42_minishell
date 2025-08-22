/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:44 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/22 03:24:19 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_match(char **result, char *filename, int *matches)
{
	char	*temp;

	if (*matches > 0)
	{
		temp = *result;
		*result = ft_strjoin(*result, " ");
		free(temp);
	}
	temp = *result;
	*result = ft_strjoin(*result, filename);
	free(temp);
	(*matches)++;
}

static int	match_pattern(const char *pattern, const char *filename)
{
	size_t	i;

	if (*pattern == '\0')
		return (*filename == '\0');
	if (*pattern == '*')
	{
		while (*(pattern + 1) == '*')
			pattern++;
		i = 0;
		while (i <= ft_strlen(filename))
		{
			if (match_pattern(pattern + 1, filename + i))
				return (1);
			i++;
		}
		return (0);
	}
	if (*filename != '\0' && (*pattern == *filename || *pattern == '?'))
		return (match_pattern(pattern + 1, filename + 1));
	return (0);
}

char	*expand_wildcard(char *str)
{
	DIR				*dir;
	struct dirent	*entry;
	char			*result;
	int				matches;

	if (!ft_strchr(str, '*'))
		return (ft_strdup(str));
	dir = opendir(".");
	if (!dir)
		return (ft_strdup(str));
	result = ft_strdup("");
	matches = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' && match_pattern(str, entry->d_name))
			add_match(&result, entry->d_name, &matches);
		entry = readdir(dir);
	}
	closedir(dir);
	if (matches == 0)
		return (free(result), ft_strdup(str));
	return (result);
}
