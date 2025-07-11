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


FIRST:
    word (can be followed by)-> epsilon, word, redir, pipe, open bracket, bin_op
    open_bracket (can be followed by)-> ), logical expr + ) 
    redir (can be followed by)-> word

ls ""||ls&&(pwd || echo -n "lakherr")| cat