/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:14:00 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/04 01:25:02 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

// Function that checks if the command is a builtin or not.
bool				ft_is_builtin(char *arg);

// Function that free the split.
void				free_split(char **s);

// Function that join two words to the env var.
void				joining_words(char **split);

// Function that return number of argument.
int					nb_args(char **args);

// Function that print export identifier error.
void				error_identifier(char **s);

// Function that check the export identifier.
int					check_identifier(char **s, int join_flag);

// Function that custome the splitted string.
char				**custome_split(char **split);

// Function that expand tild "~".
char				*custome_path(char *path);

// Function that find the first occurence of the delimiter.
int					find_delimiter(const char *str, char delimiter);

// Function that split a string using a delimiter.
char				**split_string(char *str, char delimiter);

// Check if the arg is a number or not.
int					sec_is_num(char *s);

// Function used while processing the export builtin.
int					process_equal(char **args, int i);

// Function that print the exit error case.
void				print_exit_error(char *msg);

#endif /* BUILTINS_H */