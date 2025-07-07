#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2);

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;
	size_t	size;
	size_t	i;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return ((char *)s2);
	if (!s2)
		return ((char *)s1);
	size = ft_strlen(s1) + ft_strlen(s2);
	i = 0;
	join = malloc(sizeof(char) * (size + 1));
	if (!join)
		return (NULL);
	while (*s1)
		join[i++] = *(s1++);
	while (*s2)
		join[i++] = *(s2++);
	join[i] = '\0';
	return (join);
}
