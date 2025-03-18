# Makefile for ft_ping

CC = cc
CCFLAGS = -Wall -Wextra -Werror

NAME = ft_ping

INCLUDES = -I./includes/

SRCS_MAIN = srcs/main.c

OBJS_MAIN = $(SRCS_MAIN:.c=.o)

OBJS = $(OBJS_MAIN)

%.o: %.c
	@$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CCFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

run: all
	./$(NAME)

.PHONY: all clean fclean re run