#include "libft.h"

static int	get_isize(int n)
{
	int	s;

	if (n < 0)
		n *= -1;
	s = 0;
	while (n > 0)
	{
		n /= 10;
		s++;
	}
	return (s);
}

void	ft_uitoa(char *a, int n)
{
	int		i;

	i = get_isize(n);
	a[i--] = '\0';
	while (n > 0)
	{
		a[i--] = (char)(n % 10 + 48);
		n /= 10;
	}
}

static char	*ft_imintoa(void)
{
	char	*a;

	a = malloc(sizeof(char) * 12);
	if (!a)
		return (NULL);
	a[0] = '-';
	a[1] = '2';
	ft_uitoa(a + 2, 147483648);
	return (a);
}

static char	*ft_zerotoa(void)
{
	char	*a;

	a = malloc(sizeof(char) * 2);
	if (!a)
		return (NULL);
	a[0] = '0';
	a[1] = '\0';
	return (a);
}

char	*ft_itoa(int n)
{
	char	*a;

	if (n == -2147483648)
		return (ft_imintoa());
	if (n == 0)
		return (ft_zerotoa());
	if (n < 0)
		a = malloc(sizeof(char) * (get_isize(n) + 2));
	else
		a = malloc(sizeof(char) * (get_isize(n) + 1));
	if (!a)
		return (NULL);
	if (n < 0)
	{
		n *= -1;
		a[0] = '-';
		ft_uitoa(a + 1, n);
	}
	else
	{
		ft_uitoa(a, n);
	}
	return (a);
}
