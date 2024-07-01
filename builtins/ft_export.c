/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:17:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/01 16:49:28 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->key, "_", 1))
		{
			printf("declare -x ");
			printf("%s", env->key);
			if (env->value && env->export)
				printf("=\"%s\"\n", env->value);
			else
				printf("\n");
		}
		env = env->next;
	}
}

void    ft_swap(t_env *i, t_env *j, int *swapped)
{
    char    *temp_key;
    char    *temp_value;

    temp_key = i->key;
    i->key = j->key;
    j->key = temp_key;
    temp_value = i->value;
    i->value = j->value;
    j->value = temp_value;
    *swapped = 1;
}

t_env   *new_dup(t_env *env)
{
    t_env	*head;
	t_env	*current;
	t_env	*new_node;
	int		i;

	head = NULL;
	current = NULL;
	i = 0;
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

t_env   *sort_env(t_env *env)
{
    t_env   *tmp_env;
    t_env   *i;
    t_env   *j;
    int     swapped; 
    
    tmp_env = new_dup(env);
    if (!tmp_env)
        return NULL;
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

void    free_split(char **s)
{
    int i;
    
    i = 0;
    while (s[i])
        free(s[i++]);
    free(s);
}

int check_identifier(char **s)
{
    int i;

    i = 0;
    if (!s || !*s)
        return (0);
    if (!ft_isalpha(s[0][i]))
    {
        ft_putstr_fd(RED "badashell$ : export: `=", 2);
        ft_putstr_fd(s[1], 2);
        ft_putstr_fd("`: not a valid identifier.\n" RESET, 2);
        return (-1);
        return (-1);
    }
    i++;
    while (ft_isalnum(s[0][i]))
        i++;
    if (!s[0][i])
        return (0);
    else
    {
        ft_putstr_fd(RED "badashell$ : export: `=", 2);
        ft_putstr_fd(s[1], 2);
        ft_putstr_fd("`: not a valid identifier.\n" RESET, 2);
        return (-1);
    }
}

void	ft_export(char **args, int nb_args)
{
    t_env   *sorted_env;
    char    **split;

    sorted_env = sort_env(g_minishell->our_env);
    if (nb_args == 1)
        print_env(sorted_env);
    else
    {
        if (ft_strchr(args[1], '='))
        {
            split = ft_split(args[1], '=');
            if (check_identifier(split) == -1)
            {
                clear_env(sorted_env);
                free_split(split);
                return ;
            }
            if (get_env_var(g_minishell->our_env, split[0]))
            {
                printf("*** Already in env ***\n");
                printf("*** split[1]= %s ***\n", split[1]);
                printf("*** args[2]= %s ***\n", args[2]);
                set_env_var(g_minishell->our_env, split[0], split[1], 1);
            }
            else
            {
                printf("*** New to the env ***\n");
                printf("*** split[1]= %s ***\n", split[1]);
                add_env_var(g_minishell->our_env, split[0], split[1], 1);
            }
            free_split(split);
        }
        else
        {
            if (!get_env_var(g_minishell->our_env, args[1]))
            {
                printf("*** New to the export only ***\n");
                add_env_var(g_minishell->our_env, args[1], NULL, 0);
            }
        }
    }
    clear_env(sorted_env);
}
