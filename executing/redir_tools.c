/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:14:45 by baouragh          #+#    #+#             */
/*   Updated: 2024/08/31 21:08:49 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	count_words(char *s)
{
	size_t	words;

	if (!s)
		return (0);
	words = 0;
	while (*s)
	{
		while (ft_isspace(*s))
			s++;
		if (*s)
		{
			words++;
			while (*s && !ft_isspace(*s))
				s++;
		}
	}
	return (words);
}

bool	check_expand(t_redir *new)
{
	char	*val;
	int		size;

	val = helper_expander(new->file);
	size = count_words(val);
	if (new->quoted)
		size = 1;
	if (size == 1 || !size)
	{
		if (size)
			new->file = val;
		else
			new->fd = -1;
		return (check_name(new));
	}
	else
		return (print_err("ambiguous redirect", new->file), 1);
}

int	is_ambiguous(t_redir *new)
{
	t_list	*asterisk;
	int		size;

	size = 0;
	asterisk = NULL;
	if (ft_strchr(new->file, '$'))
		return (check_expand(new));
	else if (ft_strchr(new->file, '*'))
	{
		asterisk = asterisk_functionality(new->file);
		size = ft_lstsize(asterisk);
		if (size == 1 || !size)
		{
			if (size)
				new->file = (char *)asterisk->content;
			return (check_name(new));
		}
		else
			return (print_err("ambiguous redirect", new->file), 1);
	}
	else
		return (check_name(new));
	return (0);
}

int	open_redir(t_redir *redir)
{
	if (!is_ambiguous(redir))
	{
		redir->fd = open(redir->file, redir->mode, 0644);
		if (redir->fd < 0)
		{
			ft_putstr_fd("badashell$: ", 2);
			perror(redir->file);
			g_minishell->exit_s = 1;
			set_env_var(g_minishell->our_env, "?", "1");
			return (0);
		}
		return (1);
	}
	g_minishell->exit_s = 1;
	set_env_var(g_minishell->our_env, "?", "1");
	return (0);
}

int	open_and_set(t_list *red_list)
{
	t_redir	*new;

	while (red_list)
	{
		new = red_list->content;
		if (new->type != LL_REDIR)
		{
			if (!open_redir(new))
				return (0);
		}
		red_list = red_list->next;
	}
	return (1);
}
