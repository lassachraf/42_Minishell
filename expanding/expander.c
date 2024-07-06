/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 11:11:46 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/06 20:44:43 by alassiqu         ###   ########.fr       */
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

int	contains_space(char *s)
{
	int	i;

	i = 0;
	while (s[i] && !ft_isspace(s[i]))
		i++;
	return ((s[i] != '\0'));
}

int	ft_count_words(char const *s, char c)
{
	int	words;

	words = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			words++;
			while (*s && *s != c)
				s++;
		}
	}
	return (words);
}

char	*handle_space(char *value, char *new_value)
{
	char	*new_one;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	(void)value;
	len = (ft_count_words(new_value, ' ') * 2) + ft_strlen(new_value);
	new_one = malloc(sizeof(char) * (len + 1));
	if (!new_one)
		return (NULL);
	gc_add(g_minishell, new_one);
	while (new_value[i])
	{
		if (ft_isalpha(new_value[i]) && !ft_strncmp(&new_value[i], "-", 1))
		{
			new_one[j++] = '"';
			while (new_value[i] && !ft_isspace(new_value[i]))
				new_one[j++] = new_value[i++];
			new_one[j++] = '"';
		}
		else
			new_one[j++] = new_value[i++];
	}
	new_one[j] = '\0';
	printf("new value is => '%s'\n", new_one);
	return (new_one);
}

t_token	*helper(t_token *tokens)
{
	t_token	*tmp;
	char	*new_value;

	new_value = NULL;
	if (tokens->prev && tokens->prev->type == LL_REDIR)
		return (tokens->next);
	if (tokens->prev && tokens->prev->type == D_QUOTE)
	{
		g_minishell->dq_flag = 1;
		new_value = helper_expander(tokens->value);
		if (contains_space(new_value))
			tokens->value = handle_space(tokens->value, new_value);
		else if (new_value)
			tokens->value = new_value;
		else if (!new_value)
		{
			tmp = tokens->next;
			remove_token(&g_minishell->tokens, tokens);
			return (tmp);
		}
		printf("*** new_value >>>> `%s` ***\n", tokens->value);
	}
	else
	{
		new_value = helper_expander(tokens->value);
		if (contains_space(new_value))
			tokens->value = handle_space(tokens->value, new_value);
		else if (new_value)
			tokens->value = new_value;
		else if (!new_value)
		{
			tmp = tokens->next;
			remove_token(&g_minishell->tokens, tokens);
			return (tmp);
		}
		printf("*** new_value >>>> `%s` ***\n", tokens->value);
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
