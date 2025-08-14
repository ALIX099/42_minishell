/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 20:54:10 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/13 23:33:43 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	count_pipes(t_ast *ast)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_PIPE)
		return (1 + count_pipes(ast->left) + count_pipes(ast->right));
	return (count_pipes(ast->left) + count_pipes(ast->right));
}
