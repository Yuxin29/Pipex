# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/03 16:36:29 by yuwu              #+#    #+#              #
#    Updated: 2025/06/15 18:44:12 by yuwu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := push_swap
CFLAGS := -Wall -Wextra -Werror
CC := cc

SRCS := firststep_dealinput.c \
	ft_operations.c \
	push_swap_by_size.c \
	sort_big.c \
	finalstep_main.c \

OBJS := $(SRCS:%.c=%.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re			
