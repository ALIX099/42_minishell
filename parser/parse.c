/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikarouat <ikarouat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 11:51:37 by ikarouat          #+#    #+#             */
/*   Updated: 2025/07/13 23:50:44 by ikarouat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_parser.h"

static void print_redirects(t_redirect *redir, int depth)
{
    while (redir)
    {
        for (int i = 0; i < depth; i++)
            printf("  ");
        switch (redir->type)
        {
            case REDIRECT_IN:
                printf("REDIR_IN: %s\n", redir->file);
                break;
            case REDIRECT_OUT:
                printf("REDIR_OUT: %s\n", redir->file);
                break;
            case REDIRECT_APPEND:
                printf("REDIR_APPEND: %s\n", redir->file);
                break;
            case REDIRECT_HEREDOC:
                printf("REDIR_HEREDOC: %s\n", redir->file);
                break;
        }
        redir = redir->next;
    }
}

void	print_ast(const char *direction, t_ast *ast, int depth)
{
    if (!ast)
        return;
    for (int i = 0; i < depth; i++)
        printf("%s:", direction), printf("  ");
    switch (ast->type)
    {
        case NODE_CMD:
            printf("CMD: ");
            for (int i = 0; ast->argv && ast->argv[i]; i++)
                printf("%s ", ast->argv[i]);
            printf("\n");
            print_redirects(ast->redirects, depth + 1); // <-- Add this line
            break;
        case NODE_PIPE:
            printf("PIPE\n");
            break;
        case NODE_AND:
            printf("AND\n");
            break;
        case NODE_OR:
            printf("OR\n");
            break;
        case NODE_SUBSHELL:
            printf("SUBSHELL\n");
            break;
    }
    print_ast("left", ast->left, depth + 1);
    print_ast("right", ast->right, depth + 1);
}

t_ast	*parse(char *line)
{
	t_token 	*tokens;
	t_ast	*ast;

	ast = NULL;
	//generate lexemes
	tokens = tokenize(&line);
	if (!tokens)
		return (NULL);
    //t_token *tmp;
    //tmp = tokens;
    //while (tmp)
    //{
    //    printf("Tok: %s\n", tmp->value);
    //    tmp = tmp->next;
    //}
    //Build the abstract syntax tree and check conformance to grammar
	ast = syntactic_analysis(tokens);
	if (!ast)
	{
		free(tokens);
		return (NULL);
	}
	print_ast("root", ast, 0);
	return (ast);
}