# Makefile for ft_ping

CC = cc
CCFLAGS = -Wall -Wextra -Werror

NAME = ft_ping

INCLUDES = -I./includes/

SRCS_MAIN = srcs/main.c

SRCS_UTILS = srcs/utils/time.c \
				srcs/utils/checksum.c

OBJS_MAIN = $(SRCS_MAIN:.c=.o)
OBJS_UTILS = $(SRCS_UTILS:.c=.o)

OBJS = $(OBJS_MAIN) $(OBJS_UTILS)

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
	@./$(NAME) $(ip)

.PHONY: all clean fclean re run