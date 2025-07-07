#include "libft.h"

void	ft_putnbr_fd(int n, int fd);

void	ft_print_digit(int d, int fd)
{
	char	c;

	c = d + 48;
	ft_putchar_fd(c, fd);
}

void	ft_call_if_negative(int nb, int fd)
{
	int		inv;

	ft_putchar_fd('-', fd);
	inv = nb * -1;
	ft_putnbr_fd(inv, fd);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
		write(fd, "-2147483648", 11);
	else if (n < 0)
		ft_call_if_negative(n, fd);
	else if (n < 10)
		ft_print_digit(n, fd);
	else
	{
		ft_putnbr_fd(n / 10, fd);
		ft_print_digit(n % 10, fd);
	}
}
