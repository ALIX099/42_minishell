/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:53:34 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/14 23:59:16 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast *ast)
{
	if (!ft_strcmp(ast->argv[0]->value, "env"))
		return (ast->exec->exit_value = ft_env(ast));
	else if (!ft_strcmp(ast->argv[0]->value, "unset"))
		return (ast->exec->exit_value = ft_unset(&ast->exec->my_env,
				ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "echo"))
		return (ast->exec->exit_value = ft_echo(ast, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "cd"))
		return (ast->exec->exit_value = ft_cd(ast, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "pwd"))
		return (ast->exec->exit_value = ft_pwd(ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "export"))
		return (ast->exec->exit_value = ft_export(ast, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "exit"))
		return (ast->exec->exit_value = ft_exit(ast, ast->argv));
	else
		return (ast->exec->exit_value = ft_external_cmds(ast));
	return (0);
}


// int	execute_subshell(t_ast *ast)
// {

// }

// int	execute_and(t_ast *ast)
// {

// }

// int	execute_or(t_ast *ast)
// {

// }

int	execute(t_ast *ast)
{
	// print_ast("root", ast, 0);
	if (!ast)
		return (0);
	if (ast->type == NODE_CMD)
	{
		if (ast->redirects != NULL)
			return (ast->exec->exit_value = ft_redirections(ast,
					ast->redirects));
		return (execute_command(ast));
	}
	if (ast->type == NODE_PIPE)
		return (ast->exec->exit_value = ft_pipeline(ast));
	// if (ast->type == NODE_AND || ast->type == NODE_OR)
	// 	return (execute_and(ast));
	// if (ast->type == NODE_SUBSHELL)
	// 	return (execute_subshell(ast));
	return (0); // Exit Status
}
