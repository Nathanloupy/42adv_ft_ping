# Makefile for ft_ping

CC = cc
CCFLAGS = -Wall -Wextra -Werror

NAME = ft_ping

INCLUDES = -I./includes/

SRCS_MAIN = srcs/main.c

SRCS_CORE = srcs/signals.c \
			srcs/socket.c \
			srcs/icmp.c \
			srcs/packet.c \
			srcs/timer.c \
			srcs/parser.c \
			srcs/stats.c

SRCS_UTILS = srcs/utils/time.c \
				srcs/utils/checksum.c

OBJS_MAIN = $(SRCS_MAIN:.c=.o)
OBJS_CORE = $(SRCS_CORE:.c=.o)
OBJS_UTILS = $(SRCS_UTILS:.c=.o)

OBJS = $(OBJS_MAIN) $(OBJS_CORE) $(OBJS_UTILS)

%.o: %.c
	@$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CCFLAGS) $(INCLUDES) $(OBJS) -o $(NAME) -lm

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

run: all
	@./$(NAME) $(ip)

.PHONY: all clean fclean re run