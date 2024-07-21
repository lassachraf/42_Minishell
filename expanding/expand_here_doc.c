/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_here_doc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 08:21:56 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/19 10:09:57 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	here_doc_expanding(char **s)
{
	if (ft_strchr(*s, '$'))
		*s = helper_expander(*s);
	// else if (ft_strchr(*s, '*'))
	// 	*s = ;
		
}
