/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:53:34 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/04 05:59:11 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast *ast, char **av, char **envp)
{
	if (ft_strcmp(av[0], "env"))
		get_env();
	if (ft_strcmp(av[0], "echo"))
		ft_echo();
	if (ft_strcmp(av[0], "cd"))
		ft_cd();
	if (ft_strcmp(av[0], "pwd"))
		ft_pwd();
	if (ft_strcmp(av[0], "export"))
		ft_export();
	if (ft_strcmp(av[0], "unset"))
		ft_unset();
	if (ft_strcmp(av[0], "exit"))
		ft_exit();
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

int		execute(t_ast *ast, char **av, char **envp)
{
	// print_ast("root", ast, 0);
	if (ast->type == NODE_CMD)
		return (execute_command(ast, av, envp));
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