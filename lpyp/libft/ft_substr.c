#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len);

static size_t	ft_strlen_max(const char *s, size_t max)
{
	size_t	i;

	i = 0;
	while (s[i] && i < max)
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ptr;
	size_t	i;
	size_t	p;
	size_t	mlen;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	else
		mlen = ft_strlen_max(&s[start], len);
	p = 0;
	i = start;
	ptr = malloc(sizeof(char) * (mlen + 1));
	if (!ptr)
		return (NULL);
	while (s[i] && p < mlen)
	{
		ptr[p] = s[i];
		i++;
		p++;
	}
	ptr[p] = '\0';
	return (ptr);
}
