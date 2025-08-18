/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:57:35 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/18 23:37:12 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *expand_variables_in_str(char *str)
{
    char *result = ft_strdup(str);
    char *dollar;
    
    // Find and expand all variables in the string
    while ((dollar = strchr(result, '$')) != NULL)
    {
        if (!ft_isalnum(*(dollar + 1)) && *(dollar + 1) != '_')
            break;
            
        size_t var_len = 0;
        size_t prefix_len = dollar - result;
        
        while (ft_isalnum((dollar + 1)[var_len]) || (dollar + 1)[var_len] == '_')
            var_len++;
            
        char *env_var = malloc(var_len + 1);
        ft_strlcpy(env_var, (dollar + 1), var_len + 1);
        
        char *env_val = getenv(env_var);
        if (!env_val)
            env_val = "";
            
        size_t new_len = prefix_len + ft_strlen(env_val) + 
                         ft_strlen(dollar + var_len + 1) + 1;
                         
        char *new_result = malloc(new_len);
        ft_strlcpy(new_result, result, prefix_len + 1);
        ft_strlcat(new_result, env_val, new_len);
        ft_strlcat(new_result, dollar + var_len + 1, new_len);
        
        free(env_var);
        free(result);
        result = new_result;
    }
    
    return result;
}

void	expand_node(char **arg, t_segment **segments)
{
	char *result;
    t_segment *current;
    char *expanded_segment;
    
    if (!*arg || !segments)
        return;
        
    result = ft_strdup("");
    current = segments;
    
    while (current)
    {
        // Handle segment based on its type
        if (current->state == S_QUOTED)
        {
            // Single quotes - no expansion
            expanded_segment = ft_strdup(current->seg_txt);
        }
        else if (current->state == D_QUOTED)
        {
            // Double quotes - expand variables but not wildcards
            expanded_segment = expand_variables_in_str(current->seg_txt);
        }
        else // LITERAL
        {
            // Unquoted - expand everything
            expanded_segment = expand_variables_in_str(current->seg_txt);
        }
        
        // Append expanded segment to result
        char *temp = result;
        result = ft_strjoin(result, expanded_segment);
        free(temp);
        free(expanded_segment);
        
        current = current->next;
    }
    
    // Replace original arg with concatenated result
    free(*arg);
    *arg = result;
}

void	expand_redir_list(t_redirect **redirections)
{
	t_redirect	*tmp;

	tmp = *redirections;
	while (tmp)
	{
		if (tmp->type != REDIRECT_HEREDOC)
			expand_node(&(tmp->file->value), tmp->file->segments);
		tmp = tmp->next;
	}
	return ;
}

void	expand(t_ast *ast)
{
	int	i;

	if (!ast)
		return ;
	if (ast->type == NODE_CMD)
	{
		i = 0;
		while (ast->argv[i])
		{
			
			expand_node(&ast->argv[i]->value, ast->argv[i]->segments);
			i++;
		}
		if (ast->redirects)
			expand_redir_list(&ast->redirects);
	}
	else
	{
		expand(ast->left);
		expand(ast->right);
		return ;
	}
}
