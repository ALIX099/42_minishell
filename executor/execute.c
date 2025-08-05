/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:53:34 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/05 17:58:57 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast *ast)
{
	int exit_code;

	if (!ast || !ast->argv || !ast->argv[0])
		return (0);
	if (!ft_strcmp(ast->argv[0]->value, "env"))
		exit_code = get_env(ast);
	else if (!ft_strcmp(ast->argv[0]->value, "echo"))
		exit_code = ft_echo(ast);
	else if (!ft_strcmp(ast->argv[0]->value, "cd"))
		exit_code = ft_cd(ast);
	else if (!ft_strcmp(ast->argv[0]->value, "pwd"))
		exit_code = ft_pwd(ast);
	else if (!ft_strcmp(ast->argv[0]->value, "export"))
		exit_code = ft_export(ast);
	else if (!ft_strcmp(ast->argv[0]->value, "unset"))
		exit_code = ft_unset(ast);
	else if (!ft_strcmp(ast->argv[0]->value, "exit"))
		exit_code = ft_exit(ast);
	else
		exit_code = exec_external(ast);
	return (exit_code);
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

int execute(t_ast *ast)
{
	// print_ast("root", ast, 0);
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
	return (0);//Exit Status
}