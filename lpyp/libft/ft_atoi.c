#include "libft.h"

int		ft_atoi(const char *nptr);

static int	ft_isspace(int c)
{
	return (('\t' <= c && c <= '\r') || c == ' ');
}

static int	ft_ctoi(char c)
{
	return ((int)(c - 48));
}

int	ft_atoi(const char *nptr)
{
	int		sign;
	int		nbr;
	size_t	i;

	i = 0;
	sign = 1;
	nbr = 0;
	while (nptr[i] && ft_isspace(nptr[i]))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] && ft_isdigit(nptr[i]))
		nbr = nbr * 10 + ft_ctoi(nptr[i++]);
	return (nbr * sign);
}
