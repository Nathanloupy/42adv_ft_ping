#include "lpyp.h"

/* Helper function to check if option key has been seen before */
static int check_duplicate(unsigned int *seen_keys, int seen_count, unsigned int key)
{
	int i;

	i = 0;
	while (i < seen_count)
	{
		if (seen_keys[i] == key)
			return (1);
		i++;
	}
	return (0);
}



/* Helper function to find option by short name */
static t_lpyp_option *find_short_option(t_lpyp_option *options, char c)
{
	int	i;

	i = 0;
	while (options[i].key != 0)
	{
		if (options[i].short_name == c)
			return (&options[i]);
		i++;
	}
	return (NULL);
}

/* Helper function to find option by long name */
static t_lpyp_option *find_long_option(t_lpyp_option *options, char *name)
{
	int	i;

	i = 0;
	while (options[i].key != 0)
	{
		if (options[i].long_name && ft_strncmp(options[i].long_name, name, ft_strlen(name)) == 0
			&& ft_strlen(options[i].long_name) == ft_strlen(name))
			return (&options[i]);
		i++;
	}
	return (NULL);
}

/* Helper function to handle short options */
static int handle_short_option(char c, t_lpyp_option *options, char **argv, int *i, int argc, void *data, t_lpyp_parser parser_func, unsigned int *seen_keys, int *seen_count)
{
	t_lpyp_option	*option;
	char			*argument;

	option = find_short_option(options, c);
	if (!option)
	{
		ft_putstr_fd("lpyp: invalid option -- '", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
		return (parser_func(LPYP_KEY_UNKNOWN, data, NULL));
	}

	if ((option->flags & LPYP_DENY_DUPLICATE) && check_duplicate(seen_keys, *seen_count, option->key))
	{
		ft_putstr_fd("lpyp: option '", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("' already specified\n", 2);
		return (1);
	}

	seen_keys[*seen_count] = option->key;
	(*seen_count)++;

	argument = NULL;
	if (option->flags & LPYP_REQUIRED_ARG)
	{
		if (*i + 1 >= argc || argv[*i + 1][0] == '-')
		{
			ft_putstr_fd("lpyp: option requires an argument -- '", 2);
			ft_putchar_fd(c, 2);
			ft_putstr_fd("'\n", 2);
			return (1);
		}
		(*i)++;
		argument = argv[*i];
	}
	else if (option->flags & LPYP_OPTIONAL_ARG)
	{
		if (*i + 1 < argc && argv[*i + 1][0] != '-')
		{
			(*i)++;
			argument = argv[*i];
		}
	}

	return (parser_func(option->key, data, argument));
}

/* Helper function to handle long options */
static int handle_long_option(char *arg, t_lpyp_option *options, char **argv, int *i, int argc, void *data, t_lpyp_parser parser_func, unsigned int *seen_keys, int *seen_count)
{
	t_lpyp_option	*option;
	char			*option_name;
	char			*argument;
	char			*eq_pos;

	eq_pos = ft_strchr(arg, '=');
	if (eq_pos)
		option_name = ft_substr(arg, 0, eq_pos - arg);
	else
		option_name = ft_strdup(arg);

	if (!option_name)
		return (1);

	option = find_long_option(options, option_name);
	if (!option)
	{
		ft_putstr_fd("lpyp: unrecognized option '--", 2);
		ft_putstr_fd(option_name, 2);
		ft_putstr_fd("'\n", 2);
		free(option_name);
		return (parser_func(LPYP_KEY_UNKNOWN, data, NULL));
	}

	if ((option->flags & LPYP_DENY_DUPLICATE) && check_duplicate(seen_keys, *seen_count, option->key))
	{
		ft_putstr_fd("lpyp: option '--", 2);
		ft_putstr_fd(option_name, 2);
		ft_putstr_fd("' already specified\n", 2);
		free(option_name);
		return (1);
	}

	seen_keys[*seen_count] = option->key;
	(*seen_count)++;

	argument = NULL;
	if (option->flags & LPYP_REQUIRED_ARG)
	{
		if (eq_pos)
			argument = eq_pos + 1;
		else if (*i + 1 >= argc || argv[*i + 1][0] == '-')
		{
			ft_putstr_fd("lpyp: option '--", 2);
			ft_putstr_fd(option_name, 2);
			ft_putstr_fd("' requires an argument\n", 2);
			free(option_name);
			return (1);
		}
		else
		{
			(*i)++;
			argument = argv[*i];
		}
	}
	else if (option->flags & LPYP_OPTIONAL_ARG)
	{
		if (eq_pos)
			argument = eq_pos + 1;
		else if (*i + 1 < argc && argv[*i + 1][0] != '-')
		{
			(*i)++;
			argument = argv[*i];
		}
	}

	free(option_name);
	return (parser_func(option->key, data, argument));
}

/* Main parsing function */
int lpyp_parse(void *data, int argc, char **argv, t_lpyp_option *options, t_lpyp_parser parser_func)
{
	int	i;
	int	j;
	int	result;
	unsigned int *seen_keys;
	int seen_count;
	int total_option_chars;

	total_option_chars = 0;
	i = 1;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "--", 2) == 0)
			total_option_chars += 1;
		else if (argv[i][0] == '-' && ft_strlen(argv[i]) > 1)
			total_option_chars += ft_strlen(argv[i]) - 1;
		else
			total_option_chars += 1;
		i++;
	}
	if (total_option_chars < argc)
		total_option_chars = argc;

	seen_keys = ft_calloc(total_option_chars, sizeof(unsigned int));
	if (!seen_keys)
		return (1);
	seen_count = 0;

	i = 1;
	while (i < argc)
	{
		if (ft_strncmp(argv[i], "--", 2) == 0)
		{
			if (ft_strlen(argv[i]) == 2)
			{
				i++;
				break;
			}
			result = handle_long_option(argv[i] + 2, options, argv, &i, argc, data, parser_func, seen_keys, &seen_count);
			if (result != 0)
			{
				free(seen_keys);
				return (result);
			}
		}
		else if (argv[i][0] == '-' && ft_strlen(argv[i]) > 1)
		{
			int current_i;
			
			j = 1;
			while (argv[i][j])
			{
				current_i = i;
				result = handle_short_option(argv[i][j], options, argv, &i, argc, data, parser_func, seen_keys, &seen_count);
				if (result != 0)
				{
					free(seen_keys);
					return (result);
				}
				if (i != current_i)
					break;
				j++;
			}
		}
		else
		{
			result = parser_func(LPYP_KEY_ARG, data, argv[i]);
			if (result != 0)
			{
				free(seen_keys);
				return (result);
			}
		}
		i++;
	}

	while (i < argc)
	{
		result = parser_func(LPYP_KEY_ARG, data, argv[i]);
		if (result != 0)
		{
			free(seen_keys);
			return (result);
		}
		i++;
	}

	free(seen_keys);
	return (parser_func(LPYP_KEY_END, data, NULL));
}
