/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 08:21:56 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/31 15:17:26 by alassiqu         ###   ########.fr       */
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

int	export_help(t_token **tokens, int flag)
{
	char	*tmp;

	tmp = NULL;
	if ((*tokens) && (*tokens)->value)
	{
		tmp = ft_strchr((*tokens)->value, '=');
		if (check_dollars((*tokens)->value))
		{
			check_key(&(*tokens), (*tokens)->value, tmp);
			return (1);
		}
		if (!tmp)
			return ((*tokens) = (*tokens)->next, 0);
		if (!(*(tmp + 1)))
		{
			if (export_key(&(*tokens), &tmp, flag))
				return (1);
		}
		else
			tmp++;
		if (tmp && ft_strchr(tmp, '$'))
			(*tokens)->value = helper_expander((*tokens)->value);
	}
	return (0);
}

char	*expand_without_space(char *s, int len)
{
	char	*new;
	int		i;
	int		j;

	new = malloc(sizeof(char) * (len + 1));
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	gc_add(g_minishell, new);
	while (s && s[i])
	{
		if (ft_isspace(s[i]))
		{
			new[j++] = ' ';
			while (s && s[i] && ft_isspace(s[i]))
				i++;
		}
		else
			new[j++] = s[i++];
	}
	new[j] = '\0';
	return (new);
}

char	*avoid_spaces(char *s)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	if (!s)
		return (NULL);
	while (s[i])
	{
		if (ft_isspace(s[i]))
		{
			i++;
			len++;
			while (s[i] && ft_isspace(s[i]))
				i++;
		}
		else
		{
			i++;
			len++;
		}
	}
	return (expand_without_space(s, len));
}

void	avoid_expanding(char **s, bool avoid)
{
	if (avoid)
		*s = avoid_spaces(helper_expander(*s));
	else
		*s = helper_expander(*s);
}
