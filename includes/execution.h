/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:24:37 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 19:15:05 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

// Function that get the full path of a command.
char	*get_fullpath(char *argv, char **env);

// Function that get the command.
char	*get_command(char *argv);

#endif