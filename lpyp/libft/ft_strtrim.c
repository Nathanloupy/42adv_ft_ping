#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set);

static int	is_part(char const *set, char c)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static int	get_starti(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i] && is_part(set, s1[i]))
		i++;
	return (i);
}

static int	get_endi(char const *s1, char const *set)
{
	int	i;
	int	len;

	len = (int) ft_strlen(s1);
	i = 0;
	while (i < len && is_part(set, s1[len - i - 1]))
		i++;
	return (len - i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*trim;
	int		start;
	int		end;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	start = get_starti(s1, set);
	end = get_endi(s1, set);
	if (end <= start)
		return (ft_strdup(""));
	trim = malloc(sizeof(char) * (end - start + 1));
	if (!trim)
		return (NULL);
	ft_strlcpy(trim, s1 + start, end - start + 1);
	return (trim);
}
