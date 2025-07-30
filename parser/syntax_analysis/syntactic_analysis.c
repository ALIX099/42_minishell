/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntactic_analysis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:36:27 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/29 04:26:31 by ikarouat         ###   ########.fr       */
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
    node->argv = malloc(sizeof(t_expand_arg *) * 256);//256 to be replaced with a dynamic args counter
    int argc = 0;
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
            node->argv[argc++]->expandable = (*tokens)->expandable;
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
            redir->file = malloc(sizeof(t_expand_arg));
            redir->file->value = ft_strdup((*tokens)->value);
            redir->file->expandable = (*tokens)->expandable;
            *tokens = (*tokens)->next;
            redir->next = node->redirects;
            node->redirects = redir;
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
