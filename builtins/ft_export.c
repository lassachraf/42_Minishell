/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:17:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/04 18:01:26 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env(t_env *env)
{
    if (!env)
    {
        printf("*** -- -- -- ***\n");
    }
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

void    error_identifier(char **s, char *args)
{
    ft_putstr_fd(RED "badashell$ : export: `", 2);
    ft_putstr_fd(s[0], 2);
    ft_putstr_fd("=", 2);
    if (s[1] && !args)
        ft_putstr_fd(s[1], 2);
    else if (args && !s[1])
        ft_putstr_fd(s[1], 2);
    ft_putstr_fd("`: not a valid identifier.\n" RESET, 2);
}

int check_identifier(char **s, char *args)
{
    int i;

    i = 0;
    if (!s || !*s)
        return (0);
    if (ft_isalpha(s[0][0]) || !ft_strncmp(&s[0][0], "_", 1))
        i++;
    else
        return (error_identifier(s, args), -1);
    while (ft_isalnum(s[0][i]) || !ft_strncmp(&s[0][i], "_", 1))
        i++;
    if (!s[0][i])
        return (0);
    else
        return (error_identifier(s, args), -1);
}

void	ft_swap(t_env *i, t_env *j, int *swapped)
{
	char	*temp_key;
	char	*temp_value;
	int	 temp_visblity;
	int	 temp_export;

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

// void	fill_dup(t_env *env, t_env *new_node, t_env *head, t_env *current)
// {
	
// }

t_env	*new_dup(t_env *env)
{
	t_env	*head;
	t_env	*current;
	t_env	*new_node;

	head = NULL;
	current = NULL;
	while (env)
	{
		if (!env->export)
		{
			env = env->next;
			continue;
		}
		new_node = (t_env *)malloc(sizeof(t_env));
		if (!new_node)
			return (NULL);
		new_node->key = ft_strdup(env->key);
        if (!env->value)
            new_node->value = NULL;
        else
            new_node->value = ft_strdup(env->value);
        new_node->visible = env->visible;
        new_node->export = env->export;
        new_node->next = NULL;
        if (!head)
            head = new_node;
        else
            current->next = new_node;
		current = new_node;
		env = env->next;
	}
	return (head);
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
			break;
	}
	return (tmp_env);
}

void	free_split(char **s)
{
	int i;
		
	i = 0;
	while (s[i])
		free(s[i++]);
	free(s);
}


int    process_equal(t_env *sorted_env, char **args)
{
    char    **split;

    split = ft_split(args[1], '=');
    if (check_identifier(split, args[2]) == -1)
        return (clear_env(sorted_env), free_split(split), -1);
    if (get_env_var(g_minishell->our_env, split[0]))
    {
        set_as_visible(g_minishell->our_env, split[0]);
        set_as_exported(g_minishell->our_env, split[0]);
        if (!split[1] && !args[2])
            set_env_var(g_minishell->our_env, split[0], "\0");
        else if (!split[1])
            set_env_var(g_minishell->our_env, split[0], args[2]);
        else
            set_env_var(g_minishell->our_env, split[0], split[1]);
    }
    else
    {
        if (!split[1] && !args[2])
            add_env_var(g_minishell->our_env, split[0], "\0");
        else if (!split[1])
            add_env_var(g_minishell->our_env, split[0], args[2]);
        else
            add_env_var(g_minishell->our_env, split[0], split[1]);
        set_as_exported(g_minishell->our_env, split[0]);
    }
    return (free_split(split), 0);
}

void	ft_export(char **args, int nb_args)
{
    t_env   *sorted_env;

    sorted_env = sort_env(g_minishell->our_env);
    if (nb_args == 1)
        print_env(sorted_env);
    else
    {
        if (ft_strchr(args[1], '='))
        {
            if (process_equal(sorted_env, args) == -1)
                return ;
        }
        else
        {
            printf("*** No new value case ***\n");
            if (check_identifier(&args[1], args[2]) == -1)
            {
                clear_env(sorted_env);
                return ;
            }
            else if (!get_env_var(g_minishell->our_env, args[1]))
            {
                add_env_var(g_minishell->our_env, args[1], NULL);
                set_as_invisible(g_minishell->our_env, args[1]);
            }
        }
    }
    clear_env(sorted_env);
}
