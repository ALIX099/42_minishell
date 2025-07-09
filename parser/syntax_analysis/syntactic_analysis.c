/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntactic_analysis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:36:27 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/08 23:39:00 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

//Build the ast and check the grammar conformance in the process
static t_ast *parse_and_or(t_token **tokens);
static t_ast *parse_pipeline(t_token **tokens);
static t_ast *parse_command(t_token **tokens);

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

static t_ast *parse_pipeline(t_token **tokens)
{
    t_ast *left = parse_command(tokens);
    while (*tokens && (*tokens)->type == IS_PIPE)
    {
        *tokens = (*tokens)->next;
        t_ast *right = parse_command(tokens);
        if (!right)
            return NULL;
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

static t_ast *parse_command(t_token **tokens)
{
    if (*tokens && (*tokens)->type == IS_OPEN_BRACKET)
    {
        *tokens = (*tokens)->next;
        t_ast *subshell = parse_and_or(tokens);
        if (!*tokens || (*tokens)->type != IS_CLOSE_BRACKET)
            return NULL;
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
    node->argv = malloc(sizeof(char*) * 256);
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
            node->argv[argc++] = ft_strdup((*tokens)->value);
            *tokens = (*tokens)->next;
        }
        else
        {
            t_redirect *redir = malloc(sizeof(t_redirect));
            redir->type = get_redirect_type((*tokens)->type);
            *tokens = (*tokens)->next;
            if (!*tokens || (*tokens)->type != IS_WORD)
                return NULL;
            redir->file = strdup((*tokens)->value);
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

static t_ast *parse_and_or(t_token **tokens)
{
    t_ast *left = parse_pipeline(tokens);
    while (*tokens && ((*tokens)->type == IS_AND || (*tokens)->type == IS_OR))
    {
        t_ast_type type = ((*tokens)->type == IS_AND) ? NODE_AND : NODE_OR;
        *tokens = (*tokens)->next;
        t_ast *right = parse_pipeline(tokens);
        if (!right)
            return NULL;
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

/*
    TODO:
        Add syntax error detection and grammar conformance checks
        within the parsing functions
*/
t_ast	*syntactic_analysis(t_token *tokens)
{
    return parse_and_or(&tokens);
}
