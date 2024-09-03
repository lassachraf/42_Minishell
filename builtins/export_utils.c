/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 14:40:27 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/03 19:24:13 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env(t_env *env)
{
	while (env)
	{
		if (env->export)
		{
			printf("declare -x ");
			printf("%s", env->key);
			if (env->value)
				printf("=\"%s\"\n", env->value);
			else
				printf("\n");
		}
		env = env->next;
	}
}

void	error_identifier(char **s)
{
	char	*l;

	l = ft_strchr(s[0], '=');
	ft_putstr_fd(RED "badashell$ : export : `", 2);
	if (s[0])
		ft_putstr_fd(s[0], 2);
	if (s[1] && l)
	{
		if (!ft_strncmp(s[1], "=", 1))
			ft_putstr_fd(s[1], 2);
		else
		{
			ft_putstr_fd("=", 2);
			ft_putstr_fd(s[1], 2);
		}
	}
	ft_putstr_fd("` : not a valid identifier.\n" RESET, 2);
	g_minishell->exit_s = 1;
}

int	check_identifier(char **s, int join_flag)
{
	int	i;

	i = 0;
	if (!s || !*s)
		return (0);
	if (ft_isalpha(s[0][0]) || !ft_strncmp(&s[0][0], "_", 1))
		i++;
	else
		return (error_identifier(s), -1);
	while (ft_isalnum(s[0][i]) || !ft_strncmp(&s[0][i], "_", 1))
		i++;
	if (join_flag == 1 && s[0][i] == '+')
		return (0);
	else if (join_flag == 0 && !s[0][i])
		return (0);
	else
		return (error_identifier(s), -1);
}

t_env	*sort_env(t_env *env)
{
	t_env	*tmp_env;
	t_env	*i;
	t_env	*j;
	int		swapped;

	tmp_env = new_dup(env);
	if (!tmp_env)
		return (NULL);
	while (1)
	{
		swapped = 0;
		i = tmp_env;
		while (i->next)
		{
			j = i->next;
			if (ft_strcmp(i->key, j->key) > 0)
				ft_swap(i, j, &swapped);
			i = i->next;
		}
		if (!swapped)
			break ;
	}
	return (tmp_env);
}

void	ft_swap(t_env *i, t_env *j, int *swapped)
{
	char	*temp_key;
	char	*temp_value;
	int		temp_visblity;
	int		temp_export;

	temp_key = i->key;
	i->key = j->key;
	j->key = temp_key;
	temp_value = i->value;
	i->value = j->value;
	j->value = temp_value;
	temp_visblity = i->visible;
	i->visible = j->visible;
	j->visible = temp_visblity;
	temp_export = i->export;
	i->export = j->export;
	j->export = temp_export;
	*swapped = 1;
}
