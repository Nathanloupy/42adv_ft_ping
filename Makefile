# Makefile for ft_ping

CC = cc
CCFLAGS = -Wall -Wextra -Werror

NAME = ft_ping

INCLUDES = -I./include/ -I./lpyp/include -I./lpyp/libft

LPYP_DIR = lpyp
LPYP_LIB = $(LPYP_DIR)/lpyp.a
LIBFT_LIB = $(LPYP_DIR)/libft/libft.a

SRCS_MAIN = src/main.c

SRCS_CORE = src/signals.c \
			src/socket.c \
			src/icmp.c \
			src/packet.c \
			src/timer.c \
			src/parser.c \
			src/stats.c

SRCS_UTILS = src/utils/time.c \
				src/utils/checksum.c

OBJS_MAIN = $(SRCS_MAIN:.c=.o)
OBJS_CORE = $(SRCS_CORE:.c=.o)
OBJS_UTILS = $(SRCS_UTILS:.c=.o)

OBJS = $(OBJS_MAIN) $(OBJS_CORE) $(OBJS_UTILS)

%.o: %.c
	@$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

all: $(LPYP_LIB) $(NAME)

$(LPYP_LIB):
	@make -C $(LPYP_DIR)

$(NAME): $(OBJS) $(LPYP_LIB)
	@$(CC) $(CCFLAGS) $(INCLUDES) $(OBJS) $(LPYP_LIB) $(LIBFT_LIB) -o $(NAME) -lm

clean:
	@rm -f $(OBJS)
	@make -C $(LPYP_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LPYP_DIR) fclean

re: fclean all

run: all
	@./$(NAME) $(ip)

.PHONY: all clean fclean re run