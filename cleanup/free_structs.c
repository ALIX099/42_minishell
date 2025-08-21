/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 06:32:03 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/21 23:07:18 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

void free_segment(t_segment *segment)
{
    if (!segment)
        return;
        
    if (segment->seg_txt)
        free(segment->seg_txt);
    free(segment);
}

// Free a linked list of segments
void free_segments(t_segment *segments)
{
    t_segment *curr;
    t_segment *next;

    curr = segments;
    while (curr)
    {
        next = curr->next;
        free_segment(curr);
        curr = next;
    }
}

// Free a redirect and all its components
void free_redirect(t_redirect *redirect)
{
    if (!redirect)
        return;
        
    if (redirect->file)
    {
        free(redirect->file->value);
        free_segments(redirect->file->segments);
        free(redirect->file);
    }
    
    if (redirect->heredoc)
    {
        free(redirect->heredoc->delimeter);
        free(redirect->heredoc->raw_body);
        free(redirect->heredoc);
    }
    
    free(redirect);
}

// Free a linked list of redirects
void free_redirects(t_redirect *redirects)
{
    t_redirect *curr;
    t_redirect *next;

    curr = redirects;
    while (curr)
    {
        next = curr->next;
        free_redirect(curr);
        curr = next;
    }
}

// Free an array of expand_args
void free_argv(t_expand_arg **argv)
{
    int i;

    if (!argv)
        return;
    
    i = 0;
    while (argv[i])
    {
        if (argv[i]->value)
            free(argv[i]->value);
        free_segments(argv[i]->segments);
        free(argv[i]);
        i++;
    }
    free(argv);
}

// Free an entire AST node and its components
void free_ast(t_ast *ast)
{
    if (!ast)
        return;
        
    // Recursively free child nodes
    free_ast(ast->left);
    free_ast(ast->right);
    
    // Free node components
    if (ast->redirects)
        free_redirects(ast->redirects);
        
    if (ast->argv)
        free_argv(ast->argv);
        
    // Uncomment if you need to free exec
    // if (ast->exec && ast != global_ast)
    //     free_exec(ast->exec);
    
    free(ast);
}

void free_token(t_token *token)
{
    if (!token)
        return;
        
    free(token->value);
    
    // Free segments
    t_segment *curr = token->segments;
    while (curr)
    {
        t_segment *next = curr->next;
        free(curr->seg_txt);
        free(curr);
        curr = next;
    }
    
    free(token);
}

void free_token_list(t_token *tokens)
{
    t_token *current;
    t_token *next;
    
    current = tokens;
    while (current)
    {
        next = current->next;
        free_token(current);
        current = next;
    }
}

void	free_exec(t_exec *exec)
{
	t_env *curr;
    t_env *next;
    
    curr = exec->my_env;
    while (curr)
    {
        next = curr->next;
		if (curr->key)
        	free(curr->key);
        if (curr->value)
			free(curr->value);
        free(curr);
        curr = next;
    }
    exec->my_env = NULL;
}
