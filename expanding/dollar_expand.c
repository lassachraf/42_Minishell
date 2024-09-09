/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:08:32 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/09 10:00:39 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_isnum(int c)
{
	return (c >= '0' && c <= '9');
}

int	handle_cases_len(char *expand, int *i, int *j, int *len)
{
	if (!ft_strncmp(expand, "\0", 1))
	{
		(*i) += 1;
		return ((*len) += 1, -1);
	}
	else if (!ft_strncmp(expand, "?", 1) || !ft_strncmp(expand, "_", 1))
		*j = 1;
	else if (!ft_strncmp(expand, "$", 1) || ft_isspace(*expand)
		|| !ft_isalnum(*expand) || (!ft_isalnum(*expand)
			&& ft_strncmp(expand, "_", 1)) || ft_isnum(*expand))
	{
		(*i) += 2;
		return ((*len) += 2, -1);
	}
	else
	{
		while (expand[*j])
		{
			if (ft_isalnum(expand[*j]) || !ft_strncmp(&expand[*j], "_", 1))
				(*j)++;
			else
				break ;
		}
	}
	return (0);
}

void	exit_status(char **var, int *env_len)
{
	free(*var);
	*var = ft_itoa(g_minishell->exit_s);
	*env_len = ft_strlen(*var);
}

void	handle_dollar(char *s, int *i, int *len)
{
	char	*expand;
	char	*var;
	int		env_len;
	int		j;

	expand = s + *i;
	j = 0;
	expand++;
	if (handle_cases_len(expand, i, &j, len) == -1)
		return ;
	var = ft_substr(expand, 0, j);
	if (!ft_strcmp(var, "?"))
		exit_status(&var, &env_len);
	else
		env_len = check_env(var);
	free(var);
	(*i) += j + 1;
	if (env_len != -1)
	{
		(*len) += env_len;
		return ;
	}
}
