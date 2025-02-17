/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:18:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/09 12:37:57 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	skip_zeros(char **str, int *len)
{
	if (**str && (**str == '+' || **str == '-'))
		(*str)++;
	if (**str == '0')
	{
		while (**str && **str == '0')
			(*str)++;
		if (!**str)
			(*str)--;
	}
	*len = ft_strlen(*str);
}

int	is_out_of_range(char *str)
{
	const char	*pos_limit = "9223372036854775807";
	const char	*neg_limit = "-9223372036854775808";
	int			len;

	skip_zeros(&str, &len);
	if (str[0] == '-')
	{
		if (len > 20)
			return (1);
		if (len == 20 && ft_strcmp(str, neg_limit) > 0)
			return (1);
	}
	else
	{
		if (str[0] == '+')
		{
			len--;
			str++;
		}
		if (len > 19)
			return (1);
		if (len == 19 && ft_strcmp(str, pos_limit) > 0)
			return (1);
	}
	return (0);
}

void	ft_exit(char *args, int print)
{
	int	exit_status;

	exit_status = g_minishell->exit_s;
	if (print)
		ft_putstr_fd("exit\n", 1);
	if (args && !sec_is_num(args))
	{
		print_exit_error(args);
		cleanup_minishell();
		exit(2);
	}
	if (args)
	{
		exit_status = ft_atoi(args);
	}
	if (args && is_out_of_range(args))
	{
		print_exit_error(args);
		cleanup_minishell();
		exit(2);
	}
	cleanup_minishell();
	exit(exit_status);
}
