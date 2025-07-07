#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size);

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*r;
	size_t	t;

	t = nmemb * size;
	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (t / nmemb != size)
		return (NULL);
	r = malloc(t);
	if (!r)
		return (NULL);
	ft_bzero(r, t);
	return (r);
}
