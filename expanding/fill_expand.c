/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:38:26 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/25 19:06:39 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	normal_case(char **expand, int *k)
{
	(*expand)++;
	while ((*expand)[(*k)])
	{
		if (ft_isalnum((*expand)[(*k)]) || !ft_strncmp(&(*expand)[(*k)], "_", 1))
			(*k)++;
		else
			break ;
	}
}

int	handle_cases(char **expand, int *k)
{
	if (!ft_strncmp(&(*expand)[1], "\0", 1))
		return (1);
	else if (!ft_strncmp(&(*expand)[1], "?", 1) || !ft_strncmp(&(*expand)[1], "_", 1))
	{
		(*k) = 1;
		return ((*expand)++, 2);
	}
	else if (!ft_strncmp(expand[1], "$", 1) || ft_isspace((*expand)[1])
		|| (!ft_isalnum((*expand)[1]) && ft_strncmp(&(*expand)[1], "_", 1)
		&& ft_strncmp(&(*expand)[1], "?", 1)) || !ft_isalnum((*expand)[1])
		|| ft_isnum((*expand)[1]))
		return (3);
	else
	{
		normal_case(&(*expand), k);
		return (0);
	}
	return (4);
}

void	dollar_copy(char *new, char *expanded, int *j)
{
	int	a;

	a = 0;
	while (expanded[a])
	{
		new[*j] = expanded[a++];
		(*j)++;
	}
}

char	*search_for_var(char *expand, int *i, int *k)
{
	char	*expanded;
	char	*var;

	var = ft_substr(expand, 0, (*k));
	expanded = get_env_var(g_minishell->our_env, var);
	free(var);
	(*i) += (*k) + 1;
	return (expanded);
}

void	fill_dollar(char *s, int *i, char *new, int *j)
{
	char	*expand;
	char	*expanded;
	int		k;
	int		a;

	expand = s + *i;
	k = 0;
	a = handle_cases(&expand, &k);
	if (a == 1)
	{
		new[(*j)++] = s[(*i)++];
		return ;
	}
	else if (a == 3)
	{
		new[(*j)++] = s[(*i)++];
		new[(*j)++] = s[(*i)++];
		return ;
	}
	expanded = search_for_var(expand, i, &k);
	if (!expanded)
		return ;
	else
		dollar_copy(new, expanded, j);
}
