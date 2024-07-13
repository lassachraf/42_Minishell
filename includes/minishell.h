/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:09:59 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/13 14:49:19 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_FILES_PER_LINE 6

# define PATH "/var/tmp/"
# define RED "\033[1;31m"
# define ORANGE "\033[1;33m"
# define RESET "\033[0m"
# define PROMPT "badashell$> "

# include "../libft/libft.h"
# include "builtins.h"
# include "execution.h"
# include "memory.h"
# include "parsing.h"
# include "tokenization.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

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
	int				docs;
	int				stdin;
	int				stdout;
}					t_minishell;

extern t_minishell	*g_minishell;

void				increment_shlvl(void);
int					get_exit_status(void);
char				**env_to_envp(t_env *env);
char				**list_to_argv(t_list *list);
void				printAST(t_node *node, int x, t_type type);
t_env				*special_dup_env(void);
char				*custome_path(char *path);
void				add_token_middle(t_token **tokens, t_token *new_token,
						t_token *prev_token);

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
void				ft_unset(char *key);

/* Builtins utils */

// Function that execute the builtins.
void				execute_builtins(t_minishell *mini, char **args);

// Function that checks if the command is a builtin or not.
bool				ft_is_builtin(char *arg);

// Fuction that duplcate the environment for export.
t_env				*new_dup(t_env *env);

// Function that print the export.
void				print_env(t_env *env);

// Function that sort the env to be set to export.
t_env				*sort_env(t_env *env);

// Function that swap two environment nodes.
void				ft_swap(t_env *i, t_env *j, int *swapped);

// Setters.c

// Set the env variable as exported.
void				set_as_exported(t_env *env, char *var);

// Set the env variable as unexported.
void				set_as_unexported(t_env *env, char *var);

// Set the env variable as visible.
void				set_as_visible(t_env *env, char *var);

// Set the env variable as invisible.
void				set_as_invisible(t_env *env, char *var);

/* Cleaning */

// Function that cleanup minishell for each prompt.
void				cleanup(void);

// Function that cleanup minishell.
void				cleanup_minishell(void);

//
void				clear_ast(t_node *tree);

/* Environments */

// Function that set environment variable.
void				set_env_var(t_env *env, char *var, char *new);

// Function that get the value of an environment variable.
char				*get_env_var(t_env *env, char *var);

// Function that duplicate env variable.
t_env				*dup_env(char **env);

// Function that add a new env variable.
void				add_env_var(t_env *env, char *key, char *value);

// Function that clean the environment.
void				clear_env(t_env *env);

// Function that remove an element from the env.
void				delete_env_var(t_env **env, char *key);

/* Executing */

// Main function that execute the user input.
void				executer(t_node *node);

int					here_doc(char *limiter , int doc_num);
int  				env_size(t_env *env);
char 				**env_to_envp(t_env *env);
char 				**list_to_argv(t_list *list);
void				open_pipe(int *pfd);
int					dup_2(int old_fd, int new_fd);
void				fd_duper( int *pfd , int mode);
char				*get_command(char *argv);
char				*add_slash_cmd(char *path, char *cmd);
int 				do_here_docs(t_list *red_list ,int doc_num);
int 				input_to_dup(t_list *red_list);
int 				output_to_dup(t_list *red_list);
void				run_doc_cmd(t_list *red_list);
int 				open_redir(t_redir *redir);
int 				open_and_set(t_list *red_list);
int					print_err(char *message, char *word);
void				check_split(char **cmd, char *word);
int					strings_count(char **str);
void				free_double(char **ptr);
char				*founded_cmd(char *argv, char **paths, char **cmd);
char				**get_env_paths(char **env);
char				*get_fullpath(char *argv, char **env);
int					check_cmd(char *argv, char **env);
void				call_execev(char **env, char *argv , char **cmd);
int					ft_malloc_error(char **tab, size_t i);
int 				wait_and_get(void);
void 				do_cmd(t_node *ast);
void 				do_pipe(t_node *cmd , int mode , int *pfd);
void 				execute_cmd(t_node *node);
int scan_and_set(t_node *node);
int execute_docs(t_list *red_list);
void unlink_docs(int docs);
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
void				asterisk_expand(t_token **tokens, t_token *curr);

// Function that help processing the files from directory stream.
void				process_files(t_token **tokens, t_token *prev,
						char *pattern);

// Function that help processing the directories.
void				process_dirs(DIR *dir, char **result, const char *pattern);

// Function that sort the files or the directories.
void				sort_strings(char **strings, size_t count);

// Function that match the pattern.
int					match_pattern(const char *pattern, const char *filename);

// Function that append result to the value.
void				append_to_result(char **result, char *str, int newline);

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

// Function that initalizes the signales.
void				ft_sigint_handler(int sig);

// Function that handle signals.
void				signals(void);

void	ft_sigquit_handler(int sig);

void hand(int sig);
void hand2(int sig);

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

/* Debugging ones */
void				print_tokens(t_token *token);


#endif /* MINISHELL_H */