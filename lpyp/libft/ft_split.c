#include <stdlib.h>

char	**ft_split(const char *s, char c);

static size_t	ft_word_size(const char *str, char c)
{
	size_t	size;

	size = 0;
	while (*str && *str != c)
	{
		size++;
		str++;
	}
	return (size);
}

static size_t	ft_word_count(const char *str, char c)
{
	size_t	ct;
	size_t	size;

	ct = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		size = ft_word_size(str, c);
		str += size;
		if (size != 0)
			ct++;
	}
	return (ct);
}

static char	*ft_strdup_len(const char *str, size_t len)
{
	char	*ptr;
	size_t	k;

	k = 0;
	ptr = malloc(sizeof(char) * (len + 1));
	if (!ptr)
		return (NULL);
	else
	{
		while (k < len)
		{
			ptr[k] = *str;
			str++;
			k++;
		}
		ptr[k] = '\0';
		return (ptr);
	}
}

char	**free_tab(char **tab, size_t i)
{
	size_t	k;

	k = 0;
	while (k < i)
		free(tab[k++]);
	free(tab);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	char	**tab;
	size_t	size;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	tab = malloc(sizeof(char *) * (ft_word_count(s, c) + 1));
	if (tab == NULL)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		size = ft_word_size(s, c);
		if (size != 0)
		{
			tab[i] = ft_strdup_len(s, size);
			if (tab[i++] == NULL)
				return (free_tab(tab, --i));
		}
		s += size;
	}
	tab[i] = 0;
	return (tab);
}
