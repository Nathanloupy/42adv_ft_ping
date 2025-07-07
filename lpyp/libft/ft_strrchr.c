#include "libft.h"

char	*ft_strrchr(const char *s, int c);

char	*ft_strrchr(const char *s, int c)
{
	int		i;
	char	*last;

	i = 0;
	last = NULL;
	while (s[i])
	{
		if ((unsigned char)c == s[i])
			last = (char *)(s + i);
		i++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)(s + i));
	if (last)
		return (last);
	return (NULL);
}
