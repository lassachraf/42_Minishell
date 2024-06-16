/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 11:30:18 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/16 21:32:20 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cleanup(void)
{
	gc_free_all(g_minishell);
	clear_token(&g_minishell->tokens);
	free(g_minishell->line);
}

void	cleanup_minishell(void)
{
	clear_env();
}
