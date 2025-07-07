#include "libft.h"

char	*strnstr(const char *big, const char *little, size_t len);

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;

	i = 0;
	if (*little == '\0')
		return ((char *)big);
	if (len == 0)
		return (NULL);
	while (big[i] != '\0' && i < len - (ft_strlen(little) - 1))
	{
		if (ft_strncmp(&big[i], little, ft_strlen(little)) == 0)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
