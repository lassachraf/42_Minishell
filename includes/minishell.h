/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 14:09:59 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/29 14:14:57 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <sys/stat.h>
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
	int				lines;
	int				stdin;
	int				stdout;
}					t_minishell;

extern t_minishell	*g_minishell;

// Function that handle the left side of a pipe.
void				pipe_left(t_node *node, int *pfd);

// Function that handle the right side of a pipe.
void				pipe_right(t_node *node, int *pfd);

// Function that remove null nodes.
void				remove_null(t_node **res);

// Function for debugging.
void				print_ast(const char *prefix, t_node *root, bool isLeft);

// Function that add a list inside another one.
void				add_list_into_list(t_list **lst, t_list *new);

// Function that create the name of here_doc file names.
char				*build_file_name(char *join);

// Function that check if the character is numerical.
int					ft_isnum(int c);

// Function that process and run exit builtin.
int					process_exit(char **args);

// Function that return number of argument.
int					nb_args(char **args);

// Funcion that handle space case in expanded tokens.
void				handle_space(t_token *tokens, char *new_value);

// Function that count how many word is in a string.
size_t				count_words(char *s);

// Function that checks if the here_doc should be expanded or not.
void				check_hd_expand(t_token *tokens);

// Function that fill return the new_value after expand.
void				fill_dollar(char *s, int *i, char *new, int *j);
/* Builtins */

// Function that change current working directory "cd".
void				ft_cd(t_minishell *mini, char *path);

// Function that prints the input with/without newline "echo".
void				ft_echo(char **args);

// Function that prints the env "env".
void				ft_env(t_env *env);

// Function that exit shell "exit".
void				ft_exit(char *args, int print);

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

// Function that expand tild "~".
char				*custome_path(char *path);

// Function that find the first occurence of the delimiter.
int					find_delimiter(const char *str, char delimiter);

// Function that split a string using a delimiter.
char				**split_string(char *str, char delimiter);

// Function used while processing the export builtin.
int					process_equal(char **args, int i);

// Function that print the exit error case.
void				print_exit_error(char *msg);

// Function that swap two environment nodes.
void				ft_swap(t_env *i, t_env *j, int *swapped);

// Set the env variable as exported.
void				set_as_exported(t_env *env, char *var);

// Set the env variable as unexported.
void				set_as_unexported(t_env *env, char *var);

// Set the env variable as visible.
void				set_as_visible(t_env *env, char *var);

// Set the env variable as invisible.
void				set_as_invisible(t_env *env, char *var);

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

// In case env is unset, this function set the env.
t_env				*special_dup_env(void);

/* Executing */

// Main function that execute the user input.
void				executer(t_node *node);

// Main function that handle here_doc.
int					here_doc(char *limiter, int doc_num, int expand_flag);

// Helper function for handling here_doc.
void				do_here_doc(char *buf, char *limiter, int fd, int *pipe,
						int expand_flag);

// Function that return the size of the env.
int					env_size(t_env *env);

// Function that return the env to a double pointer char.
char				**env_to_envp(t_env *env);

// Function that return the list of command to a double pointer char.
char				**list_to_argv(t_list *list);

// Function that open pipes fds.
void				open_pipe(int *pfd);

// Function that duplicate an old fd to the new one.
int					dup_2(int old_fd, int new_fd);

// Function that duplicate fd with a special mode.
void				fd_duper(int *pfd, int mode);

// Function that get the command.
char				*get_command(char *argv);

// Function that join the command with it's path.
char				*add_slash_cmd(char *path, char *cmd);

// Function that IDK XD.
int					do_here_docs(t_list *red_list);

// Function that IDK XD.
int					input_to_dup(t_list *red_list);

// Function that IDK XD.
int					output_to_dup(t_list *red_list);

// Function that IDK XD.
void				run_doc_cmd(t_list *red_list);

// Function that IDK XD.
int					open_redir(t_redir *redir);

// Function that IDK XD.
int					open_and_set(t_list *red_list);

// Function that print errors with an argument.
int					print_err(char *message, char *word);

// Function that check split returned value.
void				check_split(char **cmd, char *word);

// Function that IDK XD.
int					strings_count(char **str);

// Function that free a double pointer.
void				free_double(char **ptr);

// Function that check split returned value.
char				*founded_cmd(char *argv, char **paths, char **cmd);

// Function that get the path from the environment.
char				**get_env_paths(char **env);

// Function that check split returned value.
char				*get_fullpath(char *argv, char **env);

// Function that check for the cmd.
int					check_cmd(char *argv, char **env);

// Function that call and execute execve.
void				call_execev(char **env, char *argv, char **cmd);

// Function that is called when maslloc failed.
int					ft_malloc_error(char **tab, size_t i);

// Function that IDK XD.
int					wait_and_get(void);

// Function that IDK XD.
void				do_cmd(t_node *ast);

// Function that do pipe process.
void				do_pipe(t_node *cmd, int mode, int *pfd);

// Function that execute a command.
void				execute_cmd(t_node *node);

// Function that IDK XD.
int					scan_and_set(t_node *node);

// Function that IDK XD.
int					execute_docs(t_list *red_list);

// Function that unlink here documents.
void				unlink_docs(int docs);

// Function that read and fill here_doc buffer.
void				read_buf(char **buf, int expand_flag);

// Function that IDK XD.
int					write_or_break(int fd, char *limiter, char *buf, int count);

/* Expanding */

// Main function to do expand.
void				expanding(void);

// Function that expand inside the expanding.
void				here_doc_expanding(char **s);

// Function that return a list of nodes containing dollar expanding.
t_list				*dollar_functionality(char **s);

// Function that return a list of nodes containing asterisk expanding.
t_list				*asterisk_functionality(char *s);

// Function that help expanding words.
char				*helper_expander(char *s);

// Function that checks if the string contain spaces.
int					contains_space(char *s);

// Function that return the new value after expand.
char				*new_value(char *s, int size);

// Function that get the variable and search for it in the environment.
char				*get_var(char *s, int *i);

// Function that return the length of the variable after expanding.
int					check_env(char *var);

// Function that count the length of the whole command after expanding.
void				handle_dollar(char *s, int *i, int *len);

// Function that match the pattern.
int					match_pattern(const char *pattern, const char *filename);

/* Memory */

// Function to allocate and put the address on a garbage collector.
char				*ft_malloc(t_minishell *mini, size_t size);

// Function that add an adress to the garbage collector.
void				gc_add(t_minishell *mini, void *ptr);

// Function that free all addresses in the garbage collector.
void				gc_free_all(t_minishell *mini);

// Function that cleanup minishell.
void				cleanup_minishell(void);

// Function that clear the AST.
void				clear_ast(t_node *tree);

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

/* Signals */

// Function that initalizes the signals.
void				signals(void);

// Function that handle SIGINT signal.
void				ft_sigint_handler(int sig);

// Function that handle signals for here_doc.
void				here_doc_sig(int sig);

// Function that handle sigquit before execution.
void				ft_sigquit(int sig);

// Function that handle sigquit before execution.
void				ft_sigint(int sig);

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