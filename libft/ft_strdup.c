/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 18:44:24 by alassiqu          #+#    #+#             */
/*   Updated: 2024/06/29 17:58:41 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen(s1);
	copy = malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	if (!s1)
	{
		copy[0] = '\0';
		return (copy);
	}
	else
	{
		while (s1[i])
		{
			copy[i] = s1[i];
			i++;
		}
	}
	copy[i] = '\0';
	return (copy);
}
