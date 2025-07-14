/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:57:35 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/14 04:11:38 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_node()

int	expand(t_ast **ast)
{
	int	i;

	if (!*ast)
		return (0);//no command provided
	if ((*ast)->type == NODE_CMD)
	{
		i = 0;
		while ((*ast)->argv[i])
			expand_node((*ast)->argv[i++]);
	}
	else
	{
		//Recursivly expand cmd nodes until all nodes have been visited
		expand(&(*ast)->left);
		expand(&(*ast)->right);
	}
	return (1);
}
