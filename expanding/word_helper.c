/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 22:12:44 by alassiqu          #+#    #+#             */
/*   Updated: 2024/08/07 21:36:08 by alassiqu         ###   ########.fr       */
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

void	fill_tokens(t_token *current, char *new_value)
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
			new_tok = new_token(chunk, WORD, 0);
			new_tok->next_space = 1;
			add_token_middle(&g_minishell->tokens, new_tok, current->prev);
			current = new_tok->next;
		}
		else
			i++;
	}
}

void	handle_space(t_token *tokens, char *new_value)
{
	t_token	*current;

	current = tokens;
	if (count_words(new_value) < 2)
	{
		tokens->value = new_value;
		return ;
	}
	remove_token(&g_minishell->tokens, current);
	fill_tokens(current, new_value);
}

t_list	*dollar_functionality(char **s, bool avoid)
{
	char	**split;

	split = NULL;
	s[1] = NULL;
	avoid_expanding(s, avoid);
	if (!*s)
		return (*s = NULL, NULL);
	if (avoid)
	{
		split = ft_split(*s, ' ');
		if (!split)
			return (*s = NULL, NULL);
	}
	else
		split = s;
	return (creat_list(split, avoid));
}
