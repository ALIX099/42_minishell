/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:01:48 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/22 02:41:13 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_single_segment(t_segment *seg, t_exec *data)
{
	char	*expanded;
	char	*env_expanded;

	if (!seg || !seg->seg_txt)
		return (ft_strdup(""));
	if (seg->state == S_QUOTED)
		return (ft_strdup(seg->seg_txt));
	else if (seg->state == D_QUOTED)
	{
		expanded = expand_variables_in_str(seg->seg_txt, data);
		if (expanded)
			return (expanded);
		else
			return (ft_strdup(""));
	}
	else
	{
		env_expanded = expand_variables_in_str(seg->seg_txt, data);
		if (!env_expanded)
			return (ft_strdup(""));
		expanded = expand_wildcard(env_expanded);
		free(env_expanded);
		return (expanded);
	}
}

void	expand_node(char **arg, t_segment *segments, t_exec *data)
{
	char		*result;
	char		*expanded;
	char		*temp;
	t_segment	*current;

	if (!*arg || !segments)
		return ;
	result = ft_strdup("");
	current = segments;
	while (current)
	{
		expanded = expand_single_segment(current, data);
		temp = result;
		result = ft_strjoin(result, expanded);
		free(temp);
		free(expanded);
		current = current->next;
	}
	free(*arg);
	*arg = result;
}
