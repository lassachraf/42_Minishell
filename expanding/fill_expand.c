/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:38:26 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/09 11:43:00 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	search_and_copy(char *expand, char *new, int *j, int *k)
{
	char	*expanded;
	char	*var;
	int		i;

	i = 0;
	var = ft_substr(expand, 0, (*k));
	if (!ft_strcmp(var, "?"))
	{
		expanded = ft_itoa(g_minishell->exit_s);
		gc_add(g_minishell, expanded);
	}
	else
		expanded = get_env_var(g_minishell->our_env, var);
	free(var);
	if (!expanded)
		return ;
	else
	{
		while (expanded[i])
		{
			new[*j] = expanded[i++];
			(*j)++;
		}
	}
}

void	general_case(char **expand, int *k)
{
	(*expand)++;
	while ((*expand)[(*k)])
	{
		if (ft_isalnum((*expand)[(*k)])
			|| !ft_strncmp(&(*expand)[(*k)], "_", 1))
			(*k)++;
		else
			break ;
	}
}

int	double_copy_case(char *expand)
{
	return (!ft_strncmp(&expand[1], "$", 1) || ft_isspace(expand[1])
		|| (!ft_isalnum(expand[1]) && ft_strncmp(&expand[1], "_", 1)
			&& ft_strncmp(&expand[1], "?", 1)) || !ft_isalnum(expand[1])
		|| ft_isnum(expand[1]));
}

void	fill_dollar(char *s, int *i, char *new, int *j)
{
	char	*expand;
	int		k;

	expand = s + *i;
	k = 0;
	if (!ft_strncmp(&expand[1], "\0", 1))
	{
		new[(*j)++] = s[(*i)++];
		return ;
	}
	else if (!ft_strncmp(&expand[1], "?", 1) || !ft_strncmp(&expand[1], "_", 1))
	{
		k = 1;
		expand++;
	}
	else if (double_copy_case(expand))
	{
		(*i) += 2;
		return ;
	}
	else
		general_case(&expand, &k);
	(*i) += k + 1;
	search_and_copy(expand, new, j, &k);
}

int	to_lower(char c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
