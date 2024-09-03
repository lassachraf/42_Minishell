/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 22:12:44 by alassiqu          #+#    #+#             */
/*   Updated: 2024/09/03 08:24:49 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_env(char *var)
{
	if (!var)
		return (-1);
	if (!get_env_var(g_minishell->our_env, var))
	{
		return (-1);
	}
	else
		return (ft_strlen(get_env_var(g_minishell->our_env, var)));
}

void	fill_tokens(t_token *current, char *new_value, int flag)
{
	t_token	*new_tok;
	char	*chunk;
	int		i;
	int		j;

	i = 0;
	while (new_value[i])
	{
		if (!ft_isspace(new_value[i]))
		{
			j = i;
			while (new_value[i] && !ft_isspace(new_value[i]))
				i++;
			chunk = ft_substr(new_value, j, (i - j));
			new_tok = new_token(chunk, WORD, 0, flag);
			new_tok->next_space = 1;
			add_token_middle(&g_minishell->tokens, new_tok, current->prev);
			current = new_tok->next;
		}
		else
			i++;
	}
}

void	handle_space(t_token *tokens, char *new_value, int flag)
{
	t_token	*current;

	current = tokens;
	if (count_words(new_value) < 2)
	{
		tokens->value = new_value;
		return ;
	}
	remove_token(&g_minishell->tokens, current);
	fill_tokens(current, new_value, flag);
}

int	ft_strcasecmp(char *a, char *b)
{
	char	res_a;
	char	res_b;
	char	lower_a;
	char	lower_b;

	while (*a && *b)
	{
		lower_a = to_lower((unsigned char)*a);
		lower_b = to_lower((unsigned char)*b);
		if (lower_a != lower_b)
			return ((unsigned char)lower_a - (unsigned char)lower_b);
		a++;
		b++;
	}
	res_a = to_lower((unsigned char)*a);
	res_b = to_lower((unsigned char)*b);
	return ((unsigned char)res_a - (unsigned char)res_b);
}

int	dollar_functionality(t_list **cmds, char **s, bool quote)
{
	t_list	*list;
	char	**split;
	bool	free;

	split = NULL;
	s[1] = NULL;
	free = 0;
	avoid_expanding(s, quote);
	if (!*s)
		return (*s = NULL, 1);
	if (!quote)
	{
		free = 1;
		split = ft_split(*s, ' ');
		if (!split)
			return (*s = NULL, 1);
	}
	else
		split = s;
	list = creat_list(split, free);
	add_list_into_list(cmds, list);
	return (1);
}
