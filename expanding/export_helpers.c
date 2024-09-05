/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:36:58 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/05 14:06:25 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	export_key(t_token **tokens, char **tmp, int flag)
{
	if (ft_strchr((*tokens)->value, '$'))
	{
		*tokens = word_helper((*tokens));
		if (!flag)
			(*tokens)->prev->next_space = 0;
	}
	else
		*tokens = (*tokens)->next;
	if (!(*tokens))
		return (1);
	(*tmp) = (*tokens)->value;
	return (0);
}

int	check_dollars(char *s)
{
	int	i;
	int	key;
	int	value;

	i = -1;
	key = 0;
	value = 0;
	while (s[++i] && s[i] != '=')
	{
		if (s[i] == '$')
			key = 1;
	}
	if (s[i] && s[i] == '=')
		i++;
	if (s[i])
	{
		while (s[i])
		{
			if (s[i++] == '$')
				value = 2;
		}
	}
	return (key + value);
}

void	check_key(t_token **tokens, char *s, char *l)
{
	char	*key;
	char	*value;

	key = ft_substr(s, 0, (l - s));
	value = ft_substr(s, (l - s + 1), (ft_strlen(s) - (l - s)));
	gc_add(g_minishell, key);
	gc_add(g_minishell, value);
	if (ft_strchr(key, '$') && ft_strchr(value, '$'))
		(*tokens) = word_helper(*tokens);
	else if (ft_strchr(key, '$'))
		(*tokens) = word_helper(*tokens);
}

int	is_separator_2(char *s)
{
	return (!ft_strncmp(s, "&&", 2) || *s == '<'
		|| *s == '>' || *s == '|' || *s == '(' || *s == ')');
}
