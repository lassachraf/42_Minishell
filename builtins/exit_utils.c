/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 14:34:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/31 00:41:25 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <limits.h>

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
	while (s[i] && ft_isdigit(s[i]))
		i++;
	if (!s[i])
		return (1);
	return (0);
}

int	process_exit(char **args)
{
	if (nb_args(args) > 2)
	{
		printf("exit\n");
		if (sec_is_num(args[1]))
		{
			print_errors("exit : too many arguments");
			set_env_var(g_minishell->our_env, "?", "1");
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
		ft_exit(args[1], 1);
	set_env_var(g_minishell->our_env, "?", "0");
	g_minishell->exit_s = 0;
	return (1);
}
