/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:17:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/12 18:48:10 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	process_equal(t_env *sorted_env, char **args, int i)
{
	char	**split;

	split = ft_split(args[i], '=');
	if (!split)
		return (print_errors("Split failed in process equal"), -1);
	if (check_identifier(split, 0) == -1)
		return (clear_env(sorted_env), free_split(split), -1);
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
	return (free_split(split), 0);
}

char	**custome_split(char **split)
{
	char	*pp;
	char	*new_str;
	size_t	len;

	pp = ft_strrchr(split[0], '+');
	if (pp && *(pp + 1) == '\0')
	{
		len = pp - split[0];
		new_str = (char *)malloc((len + 1) * sizeof(char));
		if (new_str)
		{
			ft_strncpy(new_str, split[0], len);
			new_str[len] = '\0';
			free(split[0]);
			split[0] = new_str;
		}
	}
	return (split);
}

int	process_joining(t_env *sorted_env, char **args, int i)
{
	char	**split;

	split = custome_split(ft_split(args[i], '='));
	if (check_identifier(split, 0) == -1)
		return (clear_env(sorted_env), free_split(split), -1);
	if (get_env_var(g_minishell->our_env, split[0]))
	{
		joining_words(split);
	}
	else
	{
		if (!split[1])
			add_env_var(g_minishell->our_env, split[0], "\0");
		else
			add_env_var(g_minishell->our_env, split[0], split[1]);
		set_as_exported(g_minishell->our_env, split[0]);
	}
	return (free_split(split), 0);
}

void	process_each_arg(t_env *sorted, char **args, int i)
{
	if (ft_strchr(args[i], '='))
	{
		if (ft_strstr(args[i], "+="))
		{
			if (process_joining(sorted, args, i) == -1)
				return ;
		}
		else if (process_equal(sorted, args, i) == -1)
			return ;
	}
	else
	{
		if (check_identifier(&args[i], 0) == -1)
		{
			clear_env(sorted);
			return ;
		}
		else if (!get_env_var(g_minishell->our_env, args[i]))
		{
			add_env_var(g_minishell->our_env, args[i], NULL);
			set_as_invisible(g_minishell->our_env, args[i]);
		}
	}
}

void	ft_export(char **args, int nb_args)
{
	t_env	*sorted_env;
	int		i;

	sorted_env = sort_env(g_minishell->our_env);
	if (nb_args == 1)
		print_env(sorted_env);
	else
	{
		i = 1;
		while (args[i])
		{
			process_each_arg(sorted_env, args, i);
			i++;
		}
	}
	clear_env(sorted_env);
}
