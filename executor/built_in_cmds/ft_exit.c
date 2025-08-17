/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:50:06 by abouknan          #+#    #+#             */
/*   Updated: 2025/08/17 01:06:56 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

// static void	free_env_list(t_env *env)
// {
// 	t_env	*tmp;

// 	while (env)
// 	{
// 		tmp = env;
// 		env = env->next;
// 		free(tmp->key);
// 		free(tmp->value);
// 		free(tmp);
// 	}
// }

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

	argc = count_args(args);
	(void)ast;
	if (!ast->exec->is_child)
		write(1, "exit\n", 5);
	if (argc == 1)
	{
		// free_all();
		exit(ast->exec->exit_value);
	}
	if (!is_num_arg(args[1]->value))
	{
		write(2, "rsh: exit: numeric argument required\n", 37);
		// free_all();
		exit(255);
	}
	if (argc > 2)
	{
		write(2, "rsh: exit: too many arguments\n", 30);
		return (1);
	}
	// free_all();
	exit((unsigned char)ft_atoi(args[1]->value));
	return (0);
}
