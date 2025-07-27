# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/03 16:36:29 by yuwu              #+#    #+#              #
#    Updated: 2025/07/13 13:13:25 by yuwu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yuwu <yuwu@student.hive.fi>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/03 16:36:29 by yuwu              #+#    #+#              #
#    Updated: 2025/07/26 12:30:00 by yuwu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := pipex
CC := cc
CFLAGS := -Wall -Wextra -Werror -Iinclude -Ilibft/libft_0/include -Ilibft/Ft_printf/include

# Source files
SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:%.c=%.o)

# Libft setup
LIBFT_REPO := https://github.com/Yuxin29/Mini_C_Library.git
LIBFT_DIR := libft
LIBFT_MAKEFILE := $(LIBFT_DIR)/Makefile
LIBFT_A := $(LIBFT_DIR)/libft_0/libft.a
PRINTF_A := $(LIBFT_DIR)/Ft_printf/libftprintf.a
GNL_A := $(LIBFT_DIR)/Get_next_line/get_next_line.a

# Final linked libs
LIBS := $(LIBFT_A) $(PRINTF_A) $(GNL_A)

all: $(NAME)

# Force OBJS to wait until LIBS are built (i.e. clone + make done)
$(OBJS): | $(LIBS)

$(NAME): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(LIBS):
	if [ ! -d $(LIBFT_DIR) ]; then \
		echo "Cloning Mini_C_Library..."; \
		git clone $(LIBFT_REPO) $(LIBFT_DIR); \
	fi
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	[ -d $(LIBFT_DIR) ] && $(MAKE) -C $(LIBFT_DIR) clean || true

fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -rf $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
