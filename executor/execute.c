/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:53:34 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/30 05:28:21 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast *ast)
{
	
}

int	execute_pipeline(t_ast *ast)
{
	
}

int	execute_subshell(t_ast *ast)
{
	
}

int	execute_and(t_ast *ast)
{
	
}

int	execute_or(t_ast *ast)
{
	
}

int	execute(t_ast *ast)
{
	expand(ast);
	print_ast("root", ast, 0);
	if (ast->type == NODE_CMD)
		return (execute_command(ast));
	if (ast->type == NODE_PIPE)
		return (execute_pipeline(ast));
	if (ast->type == NODE_SUBSHELL)
		return (execute_subshell(ast));
	if (ast->type == NODE_AND)
		return (execute_and(ast));
	if (ast->type == NODE_OR)
		return (execute_or(ast));
	return (0);//Exit Status
}