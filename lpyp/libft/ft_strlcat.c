#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size);

static size_t	ft_strlen_max(const char *s, int enabled, size_t max)
{
	size_t	i;

	i = 0;
	while (s[i] && (enabled == 0 || (enabled == 1 && i < max)))
	{
		i++;
	}
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	d;
	size_t	s;
	size_t	t;

	t = 0;
	i = 0;
	s = ft_strlen_max(src, 0, 0);
	if (!dst && size == 0)
		return (s);
	d = ft_strlen_max(dst, 1, size);
	if (size <= 0 || d == size)
		return (d + s);
	while (i < size && dst[i])
		i++;
	while (t < size - d - 1 && src[t])
	{
		dst[i] = src[t];
		i++;
		t++;
	}
	dst[i] = '\0';
	return (d + s);
}
