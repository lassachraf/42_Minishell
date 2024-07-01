/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:09:59 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/01 19:21:38 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_FILES_PER_LINE 6

# define READLINE_LIBRARY
# define RED "\033[1;31m"
# define ORANGE "\033[1;33m"
# define RESET "\033[0m"
# define PROMPT "✨ badashell$> "

# include "../libft/libft.h"
# include "builtins.h"
# include "memory.h"
# include "parsing.h"
# include "tokenization.h"
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <sys/wait.h>
# include "execution.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			visible;
	bool			export;
	struct s_env	*next;
}					t_env;

typedef struct s_minishell
{
	char			*line;
	t_token			*tokens;
	t_env			*our_env;
	t_node			*ast;
	t_gc			*gc;
	int				nb_tokens;
	int				dq_flag;
	int				exit_s;
	int				stdin;
	int				stdout;
}					t_minishell;

extern t_minishell	*g_minishell;

void	increment_shlvl();
int		get_exit_status();
char	**env_to_envp(t_env *env);

/* Builtins */

// Function that change current working directory "cd".
void				ft_cd(t_minishell *mini, char *path);

// Function that prints the input with/without newline "echo".
void				ft_echo(char **args);

// Function that prints the env "env".
void				ft_env(t_env *env);

// Function that exit shell "exit".
void				ft_exit(char *args);

// Function that export variable to the environment "export".
void				ft_export(char **args, int nb_args);

// Function that prints current working directory "pwd".
void				ft_pwd(t_env *env);

// Function that unset environment variable "unset".
void				ft_unset(void);

/* Builtins utils */

// Function that execute the builtins.
void				execute_builtins(t_minishell *mini, char **args);

// Function that checks if the command is a builtin or not.
bool				ft_is_builtin(char *arg);

/* Cleaning */

// Function that cleanup minishell for each prompt.
void				cleanup(void);

// Function that cleanup minishell.
void				cleanup_minishell(void);

// 
void	clear_ast(t_node *tree);

/* Environments */

// Function that set environment variable.
void				set_env_var(t_env *env, char *var, char *new, bool export);

// Function that get the value of an environment variable.
char				*get_env_var(t_env *env, char *var);

// Function that duplicate env variable.
t_env				*dup_env(char **env);

// Function that add a new env variable.
void				add_env_var(t_env *env, char *key, char *value,
						bool visible);

// Function that clean the environment.
void				clear_env(t_env *env);

// Function that remove an element from the env.
void				delete_env_var(t_env **env, char *key);

/* Executing */

// Main function that execute the user input.
void				executer(t_node *node);


/* Expanding */

// Main function to do expand.
void				expander(void);

// Function that get the variable and search for it in the environment.
char				*get_var(char *s, int *i);

// Function that return the length of the variable after expanding.
int					check_env(char *var);

// Function that remove the whitespaces tokens from the list of tokens.
void				remove_whitespace(t_token **tokens);

// Funtion that prepare the commands "tokens" to be executed.
void				post_expander(void);

// Function that count the length of the whole command after expanding.
void				handle_dollar(char *s, int *i, int *len);

// Function that expand the asterisk.
void				asterisk_expand(t_token *token);

// Function that help processing the files from directory stream.
void				process_files(DIR *dir, char **result, const char *pattern);

// Function that help processing the directories.
void				process_dirs(DIR *dir, char **result, const char *pattern);

// Function that sort the files or the directories.
void				sort_strings(char **strings, size_t count);

// Function that match the pattern.
int					match_pattern(const char *pattern, const char *filename);

// Function that append result to the value.
void				append_to_result(char **result, const char *str, int newline);


/* Memory */

// Function to allocate and put the address on a garbage collector.
char				*ft_malloc(t_minishell *mini, size_t size);

// Function that add an adress to the garbage collector.
void				gc_add(t_minishell *mini, void *ptr);

// Function that free all addresses in the garbage collector.
void				gc_free_all(t_minishell *mini);

/* Nodes */

/* Nodes functions */

// Function that create a new character node.
t_node				*char_node_new(char c);

// Function that create a pair of nodes.
t_node				*pair_node_new(t_node *left, t_node *right, t_type type);

// Function that create a new string node.
t_node				*string_node_new(t_list *list);

// Function that create a new error node.
t_node				*error_node_new(const char *msg);

// Function that create a new redirection node.
t_node				*redir_node_new(t_list *red_list);

/* Parsing */

// The main function for parsing the input and return our AST structure.
t_node				*parsing(void);

// Function that parse a block or a sequence.
t_node				*parse_block(t_token **tokens);

// Function that parse a command.
t_node				*parse_cmd(t_token **tokens);

void				remove_token(t_token **head, t_token *token);

/* Signals */

// Function that handle signals.
void				signals(void);

/* Syntax */

// The main  function that checks syntax errors.
int					syntax(void);

// The function is the second phase of the detection of syntax error.
int					syntax_second_phase(t_token *token);

// The function is the third phase of the detection of syntax error.
int					syntax_third_phase(t_token *token);

// Function that check if the left expression is valid or not.
int					first_checker_left(t_token *token);

// Function that check if the right expression is valid or not.
int					first_checker_right(t_token *token);

// Function that get the number of parenthesis.
int					nb_paren(void);

// Function that get the number of the quotes.
int					nb_quotes(void);

// Function that checks if the current expression is a (PIPE || OR || AND).
int					is_pipe_or_and(t_type type);

// Function that checks if the left is valid or not.
int					is_left_valid3(t_type type);

// Function that checks if the right is valid or not.
int					is_right_valid3(t_type type);

// Function that checks if the right of parenthesis is valid or not.
int					check_right_parenthesis(t_token *token);

// Function that prints errors.
void				print_errors(char *message);

/* Tokenization */

// The main function that tokenizes the input string.
t_token				*tokenizer(void);

// Function that specifie the the type of the token.
t_token				*choose_token(char *value, char c);

// Function that append quote token to the list of token.
int					add_quote(t_token **tokens, char **line);

// Function that return if the character is a quote or not.
int					is_quote(char c);

/* Debugging ones */
void				print_tokens(t_token *token);

#endif /* MINISHELL_H */