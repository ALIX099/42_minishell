/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:53:34 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/10 21:43:31 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast *ast)
{
	if (!ft_strcmp(ast->argv[0]->value, "env"))
		return (ft_env(ast));
	else if (!ft_strcmp(ast->argv[0]->value, "unset"))
		return (ft_unset(&ast->exec->my_env, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "echo"))
		return (ft_echo(ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "cd"))
		return (ft_cd(ast, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "pwd"))
		return (ft_pwd(ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "export"))
		return(ft_export(ast, ast->argv));
	else if (!ft_strcmp(ast->argv[0]->value, "exit"))
		return (ft_exit(ast, ast->argv));
	else
		return (ft_external_cmds(ast));
	return (127);
}

// int	execute_pipeline(t_ast *ast)
// {

// }

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
	if (!ast || !ast->argv || !ast->argv[0])
		return (0);
	if (ast->type == NODE_CMD)
		return (execute_command(ast));
	// if (ast->type == NODE_PIPE)
	// 	return (execute_pipeline(ast));
	// if (ast->type == NODE_SUBSHELL)
	// 	return (execute_subshell(ast));
	// if (ast->type == NODE_AND)
	// 	return (execute_and(ast));
	// if (ast->type == NODE_OR)
	// 	return (execute_or(ast));
	return (0); // Exit Status
}