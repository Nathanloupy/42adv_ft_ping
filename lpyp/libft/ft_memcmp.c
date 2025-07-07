#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n);

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && \
		*((unsigned char *)s1 + i) == *((unsigned char *)s2 + i))
		i++;
	return ((int)(*((unsigned char *)s1 + i)) - \
	(int)(*((unsigned char *)s2 + i)));
}
