/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 20:33:35 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/29 13:23:34 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*new_value(char *s, int size)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	new = ft_malloc(g_minishell, size);
	if (!new)
		return (NULL);
	while (s[i])
	{
		if (s[i] == '$')
			fill_dollar(s, &i, new, &j);
		else
			new[j++] = s[i++];
	}
	new[j] = '\0';
	if (!new[0])
		return (NULL);
	return (new);
}

char	*helper_expander(char *s)
{
	char	*new;
	int		len;
	int		i;

	i = 0;
	len = 0;
	while (s[i])
	{
		if (s[i] == '$')
			handle_dollar(s, &i, &len);
		else
		{
			len++;
			i++;
		}
	}
	new = new_value(s, len + 1);
	return (new);
}

int	contains_space(char *s)
{
	int	i;

	i = 0;
	while (s[i] && !ft_isspace(s[i]))
		i++;
	return ((s[i] != '\0'));
}
