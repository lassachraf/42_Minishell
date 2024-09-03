/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 11:25:23 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/03 19:24:56 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_delimiter(const char *str, char delimiter)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == delimiter)
			return (i);
		i++;
	}
	return (-1);
}

void	adding_to_gc(char ***result)
{
	gc_add(g_minishell, (*result)[0]);
	gc_add(g_minishell, (*result)[1]);
	if (ft_strchr((*result)[0], '$'))
		(*result)[0] = helper_expander((*result)[0]);
	if (ft_strchr((*result)[1], '$'))
		(*result)[1] = helper_expander((*result)[1]);
}

char	**split_string(char *str, char delimiter)
{
	char	**result;
	int		index;

	result = NULL;
	index = find_delimiter(str, delimiter);
	if (index == -1)
	{
		result = malloc(2 * sizeof(char *));
		if (!result)
			return (NULL);
		gc_add(g_minishell, result);
		result[0] = ft_strdup(str);
		result[1] = NULL;
		return (result);
	}
	result = malloc(3 * sizeof(char *));
	if (!result)
		return (NULL);
	gc_add(g_minishell, result);
	result[0] = ft_strndup(str, index);
	result[1] = ft_strdup(str + index + 1);
	result[2] = NULL;
	adding_to_gc(&result);
	return (result);
}

int	process_equal(char **args, int i)
{
	char	**split;

	split = split_string(args[i], '=');
	if (!split)
		return (print_errors("Split failed in process equal !"), -1);
	if (check_identifier(split, 0) == -1)
		return (g_minishell->exit_s = 1, -1);
	if (get_env_var(g_minishell->our_env, split[0]))
	{
		set_as_visible(g_minishell->our_env, split[0]);
		set_as_exported(g_minishell->our_env, split[0]);
		if (split[1])
			set_env_var(g_minishell->our_env, split[0], split[1]);
	}
	else
	{
		if (!split[1])
			add_env_var(g_minishell->our_env, split[0], "\0");
		else
			add_env_var(g_minishell->our_env, split[0], split[1]);
		set_as_exported(g_minishell->our_env, split[0]);
	}
	return (0);
}

void	add_split_to_gc(char **split)
{
	int	i;

	i = -1;
	gc_add(g_minishell, split);
	while (split[++i])
		gc_add(g_minishell, split[i]);
}
