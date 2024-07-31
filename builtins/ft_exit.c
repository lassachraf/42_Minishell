/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:18:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/31 00:56:01 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

const char	*skip_leading_zeros(const char *str)
{
    while (*str == '0')
        str++;
    return (str);
}

int	check_negative_limit(const char *str)
{
	const char	*neg_limit = "9223372036854775808";
	size_t		len;

	str = skip_leading_zeros(str);
	len = ft_strlen(str);
	if (len > 19)
		return (1);
	if (len == 19 && ft_strcmp(str, neg_limit) > 0)
		return (1);
	return (0);
}

int	check_positive_limit(const char *str)
{
	const char	*pos_limit = "9223372036854775807";
	size_t		len;

	if (*str == '+')
		str++;
	str = skip_leading_zeros(str);
	len = ft_strlen(str);
	if (len > 19)
		return (1);
	if (len == 19 && ft_strcmp(str, pos_limit) > 0)
		return (1);
	return (0);
}

int is_out_of_range(const char *str)
{
	if (*str == '-')
		return (check_negative_limit(str + 1));
	return (check_positive_limit(str));
}

void	ft_exit(char *args, int print)
{
	int	exit_status;

	exit_status = g_minishell->exit_s;
	if (args)
	{
		exit_status = ft_atoi(args);
	}
	if (print)
		ft_putstr_fd("exit\n", 1);
	if (is_out_of_range(args))
	{
		print_exit_error(args);
		exit(2);
	}
	cleanup_minishell();
	exit(exit_status);
}
