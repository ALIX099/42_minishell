/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:06 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/22 03:13:23 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

static int	is_num_arg(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (ft_isdigit(str[i]))
		i++;
	return (str[i] == '\0');
}

int	ft_exit(t_ast *ast, t_expand_arg **args)
{
	int	argc;
	int	exit_code;

	exit_code = 0;
	argc = count_args(args);
	if (!ast->exec->is_child)
		write(1, "exit\n", 5);
	if (ast && ast->exec)
		exit_code = ast->exec->exit_value;
	if (argc == 1)
		(free_exec(ast->exec), free_ast(ast), exit(exit_code));
	if (argc > 1 && (!args[1] || !args[1]->value
			|| !is_num_arg(args[1]->value)))
	{
		write(2, "rsh: exit: numeric argument required\n", 37);
		(free_exec(ast->exec), free_ast(ast), exit(255));
	}
	if (argc > 2)
		return (write(2, "rsh: exit: too many arguments\n", 30), 1);
	exit_code = (unsigned char)ft_atoi(args[1]->value);
	free_exec(ast->exec);
	free_ast(ast);
	exit(exit_code);
}
