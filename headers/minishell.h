/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:54:13 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/30 04:34:02 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "minishell_parser.h"
#include <sys/types.h>

void	expand(t_ast *ast);
int		execute(t_ast *ast);
//To delete
void	print_ast(const char *direction, t_ast *ast, int depth);

#endif //MINISHELL_H