/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:17:11 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/27 19:31:35 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->key, "_", 1) && env->visible)
		{
			printf("declare -x ");
			printf("%s", env->key);
			if (env->value != NULL)
				printf("=\"%s\"\n", env->value);
			else
				printf("\n");
		}
		env = env->next;
	}
}
// t_env *sort_env(t_env *env)
// {
//     t_env *tmp_env = dup_env(env);
//     if (!tmp_env)
//         return NULL;

//     t_env *i, *j;
//     char *temp_key;
//     char *temp_value;

//     for (i = tmp_env; i != NULL; i = i->next)
//     {
//         for (j = i->next; j != NULL; j = j->next)
//         {
//             if (strcmp(i->key, j->key > 0))
//             {
//                 temp_key = i->key;
//                 temp_value = i->value;
//                 i->key = j->key;
//                 i->value = j->value;
//                 j->key = temp_key;
//                 j->value = temp_value;
//             }
//         }
//     }

//     return tmp_env;
// }

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

t_env   *sort_env(t_env *env)
{
    t_env   *tmp_env;
    t_env   *i;
    t_env   *j;
    int     swapped; 
    
    tmp_env = dup_env(env_to_envp(env));
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

void	ft_export(char **args, int nb_args)
{
    (void)args;
    t_env   *sorted_env;

    sorted_env = sort_env(g_minishell->our_env);
    if (nb_args == 1)
    {
        print_env(sorted_env);
        clear_env(sorted_env);
    }
    else
    {
        printf("will be processed ghi sber !\n");
    }
}
