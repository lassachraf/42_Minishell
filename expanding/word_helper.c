/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 22:12:44 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/14 15:05:17 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_var(char *s, int *i)
{
	char	*expand;
	char	*value;
	char	*var;
	int		j;

	j = 0;
	expand = s + *i + 1;
	if (!ft_strncmp(expand, "\0", 1) || ft_isspace(*expand))
		return (NULL);
	if (!ft_strncmp(expand, "$", 1))
		return (*i += 2, ft_strdup("$$"));
	if (!ft_strncmp(&expand[j], "?", 1) || !ft_strncmp(&expand[j], "_", 1))
		j = 1;
	else
		while (expand[j] && (!is_quote(expand[j]) || ft_isalnum(expand[j])
				|| !ft_strncmp(&expand[j], "_", 1)))
			j++;
	var = ft_substr(expand, 0, j);
	printf("var::: `%s`, j::: `%d`\n", var, j);
	*i += j + 1;
	value = get_env_var(g_minishell->our_env, var);
	free(var);
	if (value)
		return (value);
	else
		return (NULL);
}

int	check_env(char *var)
{
	if (!var)
		return (-1);
	if (!get_env_var(g_minishell->our_env, var))
		return (-1);
	else
		return (ft_strlen(get_env_var(g_minishell->our_env, var)));
}
