#include "lpyp.h"

/* Function to print usage information */
int lpyp_usage(t_lpyp_option *options, char *program_name)
{
	int i;
	
	ft_putstr_fd("Usage: ", 1);
	if (program_name)
		ft_putstr_fd(program_name, 1);
	else
		ft_putstr_fd("program", 1);
	
	ft_putstr_fd(" [OPTIONS]", 1);
	
	i = 0;
	while (options[i].key != 0)
	{
		if (options[i].key == LPYP_KEY_ARG)
		{
			ft_putstr_fd(" ", 1);
			if (options[i].arg_name)
				ft_putstr_fd(options[i].arg_name, 1);
			else
				ft_putstr_fd("ARGS", 1);
			break;
		}
		i++;
	}
	
	ft_putstr_fd("\n", 1);
	return (0);
}

/* Function to print detailed help */
int lpyp_help(t_lpyp_option *options, char *program_name, char *description)
{
	int i;
	int max_width;
	int current_width;
	char *option_str;
	
	lpyp_usage(options, program_name);
	
	if (description)
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd(description, 1);
		ft_putstr_fd("\n", 1);
	}
	
	max_width = 0;
	i = 0;
	while (options[i].key != 0)
	{
		current_width = 0;
		if (options[i].short_name && ft_isalpha(options[i].short_name))
			current_width += 2;
		if (options[i].short_name && ft_isalpha(options[i].short_name) && options[i].long_name)
			current_width += 2;
		if (options[i].long_name)
			current_width += 2 + ft_strlen(options[i].long_name);
		if (options[i].flags & LPYP_REQUIRED_ARG)
		{
			current_width += 1;
			if (options[i].arg_name)
				current_width += ft_strlen(options[i].arg_name);
			else
				current_width += 3;
		}
		else if (options[i].flags & LPYP_OPTIONAL_ARG)
		{
			current_width += 1;
			if (options[i].arg_name)
				current_width += 2 + ft_strlen(options[i].arg_name);
			else
				current_width += 5;
		}
		if (current_width > max_width)
			max_width = current_width;
		i++;
	}
	
	ft_putstr_fd("\nOptions:\n", 1);
	i = 0;
	while (options[i].key != 0)
	{
		option_str = ft_calloc(max_width + 50, sizeof(char));
		if (!option_str)
			return (1);
		
		current_width = 0;
		
		if (options[i].short_name && ft_isalpha(options[i].short_name))
		{
			ft_strlcat(option_str, "-", max_width + 50);
			option_str[ft_strlen(option_str)] = options[i].short_name;
			current_width += 2;
		}
		
		if (options[i].short_name && ft_isalpha(options[i].short_name) && options[i].long_name)
		{
			ft_strlcat(option_str, ", ", max_width + 50);
			current_width += 2;
		}
		
		if (options[i].long_name)
		{
			ft_strlcat(option_str, "--", max_width + 50);
			ft_strlcat(option_str, options[i].long_name, max_width + 50);
			current_width += 2 + ft_strlen(options[i].long_name);
		}
		
		if (options[i].flags & LPYP_REQUIRED_ARG)
		{
			ft_strlcat(option_str, " ", max_width + 50);
			if (options[i].arg_name)
				ft_strlcat(option_str, options[i].arg_name, max_width + 50);
			else
				ft_strlcat(option_str, "ARG", max_width + 50);
			current_width += 1 + (options[i].arg_name ? ft_strlen(options[i].arg_name) : 3);
		}
		else if (options[i].flags & LPYP_OPTIONAL_ARG)
		{
			ft_strlcat(option_str, " [", max_width + 50);
			if (options[i].arg_name)
				ft_strlcat(option_str, options[i].arg_name, max_width + 50);
			else
				ft_strlcat(option_str, "ARG", max_width + 50);
			ft_strlcat(option_str, "]", max_width + 50);
			current_width += 3 + (options[i].arg_name ? ft_strlen(options[i].arg_name) : 3);
		}
		
		ft_putstr_fd("  ", 1);
		ft_putstr_fd(option_str, 1);
		
		while (current_width < max_width)
		{
			ft_putchar_fd(' ', 1);
			current_width++;
		}
		
		if (options[i].description)
		{
			ft_putstr_fd("  ", 1);
			ft_putstr_fd(options[i].description, 1);
		}
		
		ft_putchar_fd('\n', 1);
		free(option_str);
		i++;
	}
	
	return (0);
}
