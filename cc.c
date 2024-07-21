#include "includes/minishell.h"

int	match_pattern(const char *pattern, const char *filename)
{
	while (*pattern && *filename)
	{
		if (*pattern == '*')
		{
			if (*(pattern + 1) == '\0')
				return (1);
			while (*filename)
			{
				if (match_pattern(pattern + 1, filename++))
					return (1);
			}
			return (0);
		}
		else if (*pattern != *filename)
		{
			return (0);
		}
		pattern++;
		filename++;
	}
	return ((*pattern == '*' && *(pattern + 1) == '\0')
		|| (*pattern == *filename));
}

t_list	*asterisk_functionality(char *s)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*lst;
	int				i;

	lst = NULL;
	dir = opendir(".");
	if (!dir)
		return (-1);
	i = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(s,
				entry->d_name))
		{			
			ft_lstadd_back(&lst, ft_lstnew(entry->d_name));
			i++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (lst);
}


int main(void)
{
    asterisk_functionality("*");
}
