/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 21:14:00 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 18:47:21 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

// Function that print export identifier error.
void	error_identifier(char **s);

// Function that check the export identifier.
int		check_identifier(char **s, int join_flag);

// Function that custome the splitted string.
char	**custome_split(char **split);

// Function that join two words to the env var.
void	joining_words(char **split);

// Function that free the split.
void	free_split(char **s);

#endif /* BUILTINS_H */