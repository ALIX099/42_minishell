/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 02:51:03 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 02:52:07 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_segment(t_segment *segment)
{
	if (!segment)
		return ;
	if (segment->seg_txt)
		free(segment->seg_txt);
	free(segment);
}

void	free_segments(t_segment *segments)
{
	t_segment	*curr;
	t_segment	*next;

	curr = segments;
	while (curr)
	{
		next = curr->next;
		free_segment(curr);
		curr = next;
	}
}

void	free_redirect(t_redirect *redirect)
{
	if (!redirect)
		return ;
	if (redirect->file)
	{
		free(redirect->file->value);
		free_segments(redirect->file->segments);
		free(redirect->file);
	}
	if (redirect->heredoc)
	{
		free(redirect->heredoc->delimeter);
		free(redirect->heredoc->raw_body);
		free(redirect->heredoc);
	}
	free(redirect);
}

void	free_redirects(t_redirect *redirects)
{
	t_redirect	*curr;
	t_redirect	*next;

	curr = redirects;
	while (curr)
	{
		next = curr->next;
		free_redirect(curr);
		curr = next;
	}
}
