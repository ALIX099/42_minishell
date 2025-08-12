/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 18:22:47 by macbookpro        #+#    #+#             */
/*   Updated: 2025/08/07 21:17:50 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"

int	ft_env(t_ast *ast)
{
	t_env	*tmp;

	if (!ast || !ast->exec || !ast->exec->my_env)
		return (0);
	if (count_args(ast->argv) > 1)
		return (write(2, "rsh: env: too many arguments\n", 29), 127);
	tmp = ast->exec->my_env;
	while (tmp)
	{
		if (tmp->value)
			printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
