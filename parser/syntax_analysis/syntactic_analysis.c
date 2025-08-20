/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntactic_analysis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:36:27 by ikarouat          #+#    #+#             */
/*   Updated: 2025/08/20 17:32:43 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static t_redirect_type  get_redirect_type(t_token_type tok_type)
{
    if (tok_type == IS_REDIRECT_IN)
        return REDIRECT_IN;
    if (tok_type == IS_REDIRECT_OUT)
        return REDIRECT_OUT;
    if (tok_type == IS_REDIRECT_APPEND)
        return REDIRECT_APPEND;
    if (tok_type == IS_REDIRECT_HEREDOC)
        return REDIRECT_HEREDOC;
    return (-1);
}

size_t  count_tok_args(t_token *tokens)
{
    size_t  ret;
    t_token *tmp;

    ret = 0;
    tmp = tokens;
    while (tmp && (tmp->type == IS_WORD ||
        tmp->type == IS_REDIRECT_IN ||
        tmp->type == IS_REDIRECT_OUT ||
        tmp->type == IS_REDIRECT_APPEND ||
        tmp->type == IS_REDIRECT_HEREDOC))
    {
        if (tmp->type == IS_WORD)
            ++ret;
        else if (tmp->next)
            tmp = tmp->next;
        tmp = tmp->next;
    }
    return (ret);
}

t_ast *parse_command(t_token **tokens)
{
    if (!*tokens || ((*tokens)->type != IS_WORD &&
        (*tokens)->type != IS_REDIRECT_IN &&
        (*tokens)->type != IS_REDIRECT_OUT &&
        (*tokens)->type != IS_REDIRECT_APPEND &&
        (*tokens)->type != IS_REDIRECT_HEREDOC &&
        (*tokens)->type != IS_OPEN_BRACKET))
    {
        syntax_error("unexpected token", *tokens);
        return NULL;
    }
    if (*tokens && (*tokens)->type == IS_OPEN_BRACKET)
    {
        *tokens = (*tokens)->next;
        t_ast *subshell = parse_logical_expr(tokens);
        if (!*tokens || (*tokens)->type != IS_CLOSE_BRACKET)
        {
            return (syntax_error("expected closing parenthesis", *tokens), NULL);
        }
        *tokens = (*tokens)->next;
        t_ast *node = malloc(sizeof(t_ast));
        node->type = NODE_SUBSHELL;
        node->left = subshell;
        node->right = NULL;
        node->argv = NULL;
        node->redirects = NULL;
        return node;
    }

    t_ast *node = malloc(sizeof(t_ast));
    node->type = NODE_CMD;
    node->argv = malloc(sizeof(t_expand_arg *) * count_tok_args(*tokens) + 1);//dynamic args counter
    size_t  argc = 0;
    node->redirects = NULL;
    while (*tokens && ((*tokens)->type == IS_WORD ||
                       (*tokens)->type == IS_REDIRECT_IN ||
                       (*tokens)->type == IS_REDIRECT_OUT ||
                       (*tokens)->type == IS_REDIRECT_APPEND ||
                       (*tokens)->type == IS_REDIRECT_HEREDOC))
    {
        if ((*tokens)->type == IS_WORD)
        {
            node->argv[argc] = malloc(sizeof(t_expand_arg)); 
            node->argv[argc]->value = ft_strdup((*tokens)->value);
            node->argv[argc++]->segments = (*tokens)->segments;
            *tokens = (*tokens)->next;
        }
        else
        {
            t_redirect *redir = malloc(sizeof(t_redirect));
            redir->type = get_redirect_type((*tokens)->type);
            *tokens = (*tokens)->next;
            if (!*tokens || (*tokens)->type != IS_WORD)
            {
                return (syntax_error("expected filename after redirection", *tokens), NULL);
            }
            if (redir->type == REDIRECT_HEREDOC)
            {
                // Allocate heredoc structure
                redir->heredoc = malloc(sizeof(t_heredoc));
                if (!redir->heredoc)
                    return NULL;
                        
                // Store delimiter and check if it's quoted
                char *delimiter = (*tokens)->value;
                int is_quoted = 0;
                        
                // Check if delimiter is quoted (for expansion control)
                t_segment *seg = (*tokens)->segments;
                while (seg)
                {
                    if (seg->state == S_QUOTED || seg->state == D_QUOTED)
                    {
                        is_quoted = 1;
                        break;
                    }
                    seg = seg->next;
                }
                
                // Setup heredoc properties
                redir->heredoc->delimeter = ft_strdup(delimiter);
                redir->heredoc->quoted = is_quoted;
                redir->heredoc->fd = -1;  // Will be set during execution
                redir->heredoc->raw_body = NULL;  // Will be filled during execution
                
                // Still allocate file structure for consistency
                redir->file = malloc(sizeof(t_expand_arg));
                redir->file->value = ft_strdup((*tokens)->value);
                redir->file->segments = (*tokens)->segments;
            }
            else
            {
                redir->file = malloc(sizeof(t_expand_arg));
                redir->file->value = ft_strdup((*tokens)->value);
                redir->file->segments = (*tokens)->segments;
                redir->heredoc = 0;
            }
            ft_redirlist_add_back(&(node->redirects), &redir);
            *tokens = (*tokens)->next;
        }
    }
    node->argv[argc] = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

t_ast *parse_pipeline(t_token **tokens)
{
    t_ast *left = parse_command(tokens);
    while (*tokens && (*tokens)->type == IS_PIPE)
    {
        *tokens = (*tokens)->next;
        t_ast *right = parse_command(tokens);
        if (!right)
            return (syntax_error("expected command after operator", *tokens), NULL);
        t_ast *parent = malloc(sizeof(t_ast));
        parent->type = NODE_PIPE;
        parent->right = right;
        parent->argv = NULL;
        parent->left = left;
        parent->redirects = NULL;
        left = parent;
    }
    return left;
}

t_ast *parse_logical_expr(t_token **tokens)
{
    t_ast *left = parse_pipeline(tokens);
    while (*tokens && ((*tokens)->type == IS_AND || (*tokens)->type == IS_OR))
    {
        t_ast_type type = ((*tokens)->type == IS_AND) ? NODE_AND : NODE_OR;
        *tokens = (*tokens)->next;
        t_ast *right = parse_pipeline(tokens);
        if (!right)
            return (syntax_error("expected command after logical operator", *tokens), NULL);
        t_ast *parent = malloc(sizeof(t_ast));
        parent->type = type;
        parent->left = left;
        parent->right = right;
        parent->argv = NULL;
        parent->redirects = NULL;
        left = parent;
    }
    return left;
}

t_ast	*syntactic_analysis(t_token *tokens)
{
    t_ast   *ast;

    ast = parse_logical_expr(&tokens);//A recursive descent parser
    if (!ast)
        return (NULL);
    if (tokens)
    {
        syntax_error("unexpected token at end of input", tokens); 
        return (NULL);
    }
    return (ast);
}