/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:11:46 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/26 10:06:02 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	fill_dollar(char *s, int *i, char *new, int *j)
{
	char	*expand;
	char	*expanded;
	char	*var;
	int		k;
	int		a;

	expand = s + *i;
	k = 0;
	a = 0;
	if (!ft_strncmp(&expand[1], "\0", 1))
	{
		new[(*j)++] = s[(*i)++];
		return ;
	}
	else if (!ft_strncmp(&expand[1], "?", 1))
	{
		k = 1;	
		expand++;
	}
	else if (!ft_strncmp(expand + 1, "$", 1) || ft_isspace(expand[1]) || (!ft_isalnum(expand[1]) && ft_strncmp(&expand[1], "_", 1) && ft_strncmp(&expand[1], "?", 1)) || !ft_isalnum(expand[1]))
	{
		new[(*j)++] = s[(*i)++];
		new[(*j)++] = s[(*i)++];
		return ;
	}
	else
	{
		expand++;
		while (expand[k])
		{
			if (ft_isalnum(expand[k]) || !ft_strncmp(&expand[k], "_", 1))
				k++;
			else
				break ;
		}
	}
	var = ft_substr(expand, 0, k);
	expanded = get_env_var(g_minishell->our_env, var);
	free(var);
	(*i) += k + 1;
	if (!expanded)
		return ;
	else
	{
		while (expanded[a])
		{
			new[*j] = expanded[a++];
			(*j)++;
		}
	}
}

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

t_token	*helper(t_token *tokens)
{
	t_token	*tmp;
	char	*new_value;

	new_value = NULL;
	if (tokens->prev && tokens->prev->type == D_QUOTE)
	{
		g_minishell->dq_flag = 1;
		new_value = helper_expander(tokens->value);
		if (new_value)
		{
			// free(tokens->value);
			tokens->value = new_value;
		}
		else
		{
			tmp = tokens->next;
			remove_token(&g_minishell->tokens, tokens);
			return (tmp);
		}
	}
	else
	{
		new_value = helper_expander(tokens->value);
		if (new_value)
		{
			// free(tokens->value);
			tokens->value = new_value;
		}
		else
		{
			tmp = tokens->next;
			remove_token(&g_minishell->tokens, tokens);
			return (tmp);
		}
	}
	return (tokens->next);
}

void	expanding(void)
{
	t_token	*tokens;

	tokens = g_minishell->tokens;
	while (tokens)
	{
		g_minishell->dq_flag = 0;
		if (tokens->type == S_QUOTE)
		{
			tokens = tokens->next;
			while (tokens && tokens->type != S_QUOTE)
				tokens = tokens->next;
		}
		else if (tokens->type == ASTERISK)
			asterisk_expand(tokens);
		else if (tokens->type == WORD && ft_strchr(tokens->value, '$'))
		    tokens = helper(tokens);
        else
            tokens = tokens->next;
	}
}

void	expander(void)
{
	expanding();
	post_expander();
}
