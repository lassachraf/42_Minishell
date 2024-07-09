/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 12:43:25 by alassiqu          #+#    #+#             */
/*   Updated: 2024/07/09 13:09:16 by alassiqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	needle_len;
	size_t	haystack_len;

	needle_len = strlen(needle);
	haystack_len = strlen(haystack);
	if (needle_len == 0)
		return ((char *)haystack);
	while (*haystack && (haystack + needle_len <= haystack + haystack_len))
	{
		if (strncmp(haystack, needle, needle_len) == 0)
			return ((char *)haystack);
		haystack++;
	}
	return (NULL);
}
