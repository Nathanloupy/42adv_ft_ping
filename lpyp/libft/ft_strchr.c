#include "libft.h"

char	*ft_strchr(const char *s, int c);

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] && (unsigned char)c != s[i])
	{
		i++;
	}
	if ((unsigned char)c == s[i])
		return ((char *)(s + i));
	return (NULL);
}
