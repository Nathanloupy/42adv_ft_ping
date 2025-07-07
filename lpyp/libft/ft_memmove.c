#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n);

static int	isoverlapping(void *dest, const void *src, size_t n)
{
	if (src <= dest && dest <= src + n)
		return (-1);
	if (dest <= src && src <= dest + n)
		return (1);
	else
		return (0);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	int	r;
	int	i;

	if (n <= 0 || (!dest && !src))
		return (dest);
	r = isoverlapping(dest, src, n);
	if (r != 0)
	{
		if (r == 1)
			i = 0;
		else
			i = n - 1;
		while ((r == 1 && (size_t) i < n) || (r == -1 && i >= 0))
		{
			ft_memcpy(dest + i, src + i, 1);
			i += r;
		}
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}
