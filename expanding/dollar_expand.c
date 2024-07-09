/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 09:08:32 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/09 10:23:17 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_dollar(char *s, int *i, int *len)
{
	char	*expand;
	char	*var;
	int		env_len;
	int		j;

	expand = s + *i;
	j = 0;
	expand++;
	if (!ft_strncmp(expand, "\0", 1))
	{
		*i += 1;
		(*len) += 1;
		return ;
	}
	else if (!ft_strncmp(expand, "?", 1) || !ft_strncmp(expand, "_", 1))
		j = 1;
	else if (!ft_strncmp(expand, "$", 1) || ft_isspace(*expand) || !ft_isalnum(*expand) || (!ft_isalnum(*expand) && ft_strncmp(expand, "_", 1)))
	{
		*i += 2;
		(*len) += 2;
		return ;
	}
	else
	{
		while (expand[j])
		{
			if (ft_isalnum(expand[j]) || !ft_strncmp(&expand[j], "_", 1))
				j++;
			else
				break ;
		}
	}
	var = ft_substr(expand, 0, j);
	env_len = check_env(var);
	free(var);
	(*i) += j + 1;
	if (env_len == -1)
		return ;
	else
	{
		(*len) += env_len;
		return ;
	}
}
