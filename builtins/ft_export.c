/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:17:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/08 10:03:44 by alassiqu         ###   ########.fr       */
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
    if (s[0])
        ft_putstr_fd(s[0], 2);
    ft_putstr_fd("=", 2);
    if (s[1] && !args)
        ft_putstr_fd(s[1], 2);
    else if (args && !s[1])
        ft_putstr_fd(args, 2);
    ft_putstr_fd("`: not a valid identifier.\n" RESET, 2);
}

int check_identifier(char **s, char *args, int join_flag)
{
    int i;

    i = 0;
    if (!s || !*s)
        return (0);
    printf("** s[0] >>>> `%s` **\n", s[0]);
    if (ft_isalpha(s[0][0]) || !ft_strncmp(&s[0][0], "_", 1))
        i++;
    else
        return (error_identifier(s, args), -1);
    while (ft_isalnum(s[0][i]) || !ft_strncmp(&s[0][i], "_", 1))
        i++;
    printf("** s[0] >>>> `%c`, join_flag >>> `%d` **\n", s[0][i], join_flag);
    if (join_flag == 1 && s[0][i] == '+')
        return (0);
    else if (join_flag == 0 && !s[0][i])
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
    if (!split)
        return (error_identifier(NULL, args[1]), -1);
    if (check_identifier(split, args[2], 0) == -1)
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

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		++i;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char    **custome_split(char **split)
{
    char *pp;
    char *new_str;
    size_t len;

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

int    process_joining(t_env *sorted_env, char **args)
{
    char    **split;
    char    *temp;
    char    *new;

    new = NULL;
    split = ft_split(args[1], '=');
    split = custome_split(split);
    if (check_identifier(split, args[2], 0) == -1)
        return (clear_env(sorted_env), free_split(split), -1);
    if (get_env_var(g_minishell->our_env, split[0]))
    {
        set_as_visible(g_minishell->our_env, split[0]);
        set_as_exported(g_minishell->our_env, split[0]);
        temp = get_env_var(g_minishell->our_env, split[0]);
        if (!split[1] && args[2])
            new = ft_strjoin(temp, args[2]);
        else if (split[1] && !args[2])
            new = ft_strjoin(temp, split[1]);
        gc_add(g_minishell, new);
        set_env_var(g_minishell->our_env, split[0], new);
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
    // int     i;

    sorted_env = sort_env(g_minishell->our_env);
    printf("*** nb_args >>>>>>> `%d` ***\n", nb_args);
    if (nb_args == 1)
        print_env(sorted_env);
    else
    {
        // Should be a while ...
        if (args[1])
        {
            if (ft_strchr(args[1], '='))
            {
                if (ft_strstr(args[1], "+="))
                {
                    if (process_joining(sorted_env, args) == -1)
                        return ;
                }
                else if (process_equal(sorted_env, args) == -1)
                    return ;
            }
            else
            {
                if (check_identifier(&args[1], args[2], 0) == -1)
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
            // i++;
        }
    }
    clear_env(sorted_env);
}

void	ft_exportbvsdf(char **args, int nb_args)
{
    t_env   *sorted_env;
    int     i;

    sorted_env = sort_env(g_minishell->our_env);
    printf("*** nb_args >>>>>>> `%d` ***\n", nb_args);
    if (nb_args == 1)
        print_env(sorted_env);
    else
    {
        i = 1;
        while (args[i])
        {
            if (ft_strchr(args[i], '='))
            {
                if (ft_strstr(args[i], "+="))
                {
                    if (process_joining(sorted_env, args) == -1)
                        return ;
                }
                else if (process_equal(sorted_env, args) == -1)
                    return ;
            }
            else
            {
                if (check_identifier(&args[i], args[i + 1], 0) == -1)
                {
                    clear_env(sorted_env);
                    return ;
                }
                else if (!get_env_var(g_minishell->our_env, args[i]))
                {
                    add_env_var(g_minishell->our_env, args[i], NULL);
                    set_as_invisible(g_minishell->our_env, args[i]);
                }
            }
            i += 2;
        }
    }
    clear_env(sorted_env);
}
