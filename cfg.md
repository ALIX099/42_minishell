# Minishell CFG

## Terminals (Tokens)
| Token            | Lexical Pattern       | Example         |
|------------------|-----------------------|-----------------|
| `WORD`           | `[^<>|&() \t\n]+`    | `"ls"`, `"-l"`  |
| `PIPE`           | `|`                   | `|`             |
| `REDIR_IN`       | `<`                   | `<`             |
| `REDIR_OUT`      | `>`                   | `>`             |
| `REDIR_APPEND`   | `>>`                  | `>>`            |
| `REDIR_HEREDOC`  | `<<`                  | `<<`            |
| `OPEN_BRACKET`   | `(`                   | `(`             |
| `CLOSE_BRACKET`  | `)`                   | `)`             |
| `AND`            | `&&`                  | `&&`            |
| `OR`             | `||`                  | `||`            |

## Non-Terminals
| Non-Terminal       | Description                          |
|--------------------|--------------------------------------|
| `program`          | Root production                      |
| `logical_expression`| Handles `&&`, `||`, and `( )` groups |
| `pipeline`         | Chains commands with `|`            |
| `command`          | Simple command or redirection        |
| `redirection`      | File redirection syntax              |

## Production Rules
program          = logical_expression

logical_expression 
                = pipeline ( ("&&" | "||") pipeline )*
                | "(" logical_expression ")" ( ("&&" | "||") pipeline )*

pipeline        = command ( "|" command )*

command         = (redirection)* word(1,*) (redirection)*

redirection     = ("<" | ">" | ">>" | "<<") word

NOTE:
    Expander and executor to be implemented using (Pre-Order)BFS
