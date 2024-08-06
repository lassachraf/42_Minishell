/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 14:34:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/06 18:27:36 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_exit_error(char *msg)
{
	ft_putstr_fd(RED "badashell$ : exit : ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(" : numeric argument required.\n" RESET, 2);
}

int	sec_is_num(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	while (s[i] && ft_isdigit(s[i]))
		i++;
	if (!s[i])
		return (1);
	return (0);
}

int	process_exit(char **args, bool print)
{
	if (nb_args(args) > 2)
	{
		if (print)
			ft_putstr_fd("exit\n", 1);
		if (sec_is_num(args[1]))
		{
			print_errors("exit : too many arguments.");
			g_minishell->exit_s = 1;
		}
		else
		{
			print_exit_error(args[1]);
			g_minishell->exit_s = 2;
			ft_exit("2", 0);
		}
		return (0);
	}
	else
		ft_exit(args[1], print);
	g_minishell->exit_s = 0;
	return (1);
}
