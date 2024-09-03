/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:17:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/03 19:20:38 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**custome_split(char **split)
{
	char	*pp;
	char	*new_str;
	size_t	len;

	add_split_to_gc(split);
	pp = ft_strrchr(split[0], '+');
	if (pp && *(pp + 1) == '\0')
	{
		len = pp - split[0];
		new_str = (char *)malloc((len + 1) * sizeof(char));
		gc_add(g_minishell, new_str);
		if (new_str)
		{
			ft_strncpy(new_str, split[0], len);
			new_str[len] = '\0';
			split[0] = new_str;
		}
	}
	if (split[0] && ft_strchr(split[0], '$'))
		split[0] = helper_expander(split[0]);
	if (split[1] && ft_strchr(split[1], '$'))
		split[1] = helper_expander(split[1]);
	return (split);
}

int	process_joining(char **args, int i)
{
	char	**split;

	split = custome_split(ft_split(args[i], '='));
	if (!split[0][0])
	{
		split[0] = ft_strdup("+=");
		gc_add(g_minishell, split[0]);
	}
	if (check_identifier(split, 0) == -1)
		return (g_minishell->exit_s = 1, -1);
	if (get_env_var(g_minishell->our_env, split[0]))
		joining_words(split);
	else
	{
		if (!split[1])
			add_env_var(g_minishell->our_env, split[0], "\0");
		else
			add_env_var(g_minishell->our_env, split[0], split[1]);
		set_as_exported(g_minishell->our_env, split[0]);
	}
	set_env_var(g_minishell->our_env, "?", "0");
	return (g_minishell->exit_s = 0, 0);
}

int	case_of_no_value(char **args, int *i)
{
	if (check_identifier(&args[(*i)], 0) == -1)
	{
		g_minishell->exit_s = 1;
		return (1);
	}
	else if (!get_env_var(g_minishell->our_env, args[(*i)]))
	{
		add_env_var(g_minishell->our_env, args[(*i)], NULL);
		set_as_invisible(g_minishell->our_env, args[(*i)]);
	}
	return (0);
}

int	process_each_arg(char **args, int i)
{
	if (ft_strchr(args[i], '='))
	{
		if (ft_strstr(args[i], "+="))
		{
			if (process_joining(args, i) == -1)
				return (1);
		}
		else if (process_equal(args, i) == -1)
		{
			g_minishell->exit_s = 1;
			set_env_var(g_minishell->our_env, "?", "1");
			return (1);
		}
	}
	else
	{
		if (case_of_no_value(args, &i))
			return (1);
	}
	return (0);
}

void	ft_export(char **args, int nb_args)
{
	t_env	*sorted_env;
	int		i;
	int		j;

	j = 0;
	sorted_env = sort_env(g_minishell->our_env);
	if (nb_args == 1)
		print_env(sorted_env);
	else
	{
		i = 1;
		while (args[i])
		{
			j += process_each_arg(args, i);
			i++;
		}
	}
	if (j == 0)
	{
		g_minishell->exit_s = 0;
		set_env_var(g_minishell->our_env, "?", "0");
	}
	clear_env(sorted_env);
}
