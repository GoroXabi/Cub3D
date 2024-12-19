# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: xortega <xortega@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/05 11:02:31 by xortega           #+#    #+#              #
#    Updated: 2024/12/19 14:43:54 by xortega          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	cubed
OBJ_DIR		:=	obj/
SRC_PATH	:=	src/
LIBFT_PATH	:=	libft
LIBFT		:=	libft.a
MLX			:=	libmlx42.a
MLX_PATH	:=	MLX42/build
LIBMLX		:=	./MLX42
OBJS_FAGS	:=	$(MLX) -ldl -lglfw -pthread -lm
CC			:=	gcc
CFLAGS		:=	-Wextra -Wall -Werror -Wunreachable-code -Ofast -Iincludes -g3

# SRCS #
GRAPHICS	:= resize print
PLAYER		:= player
MAIN		:= main
RAYS	 	:= rays math
PARSING	 	:= parsing parsero psycho_path map_mutate


PLAIN_SRCS 	:=	$(addsuffix .c, $(addprefix main/,		$(MAIN))) \
				$(addsuffix .c, $(addprefix rays/,		$(RAYS))) \
				$(addsuffix .c, $(addprefix parsing/,	$(PARSING))) \
				$(addsuffix .c, $(addprefix player/,	$(PLAYER))) \
				$(addsuffix .c, $(addprefix graphics/,	$(GRAPHICS)))

SRCS 		:= $(addprefix $(SRC_PATH), $(PLAIN_SRCS))
OBJS 		:= $(addprefix $(OBJ_DIR), $(PLAIN_SRCS:.c=.o))


# ASCII COLORS #
BLACK=\033[0;30m
RED=\033[0;31m
#RED=\033[38;5;208m
GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
MAG=\033[0;35m
CYAN=\033[38;5;6m
#WHITE=\033[1;37m
WHITE=\033[38;5;255m
PAPYRUS=\033[38;5;223m
PINK=\033[38;5;213m
END=\033[0m
WHITEB=\033[48;5;255m
CYANB=\033[48;5;6m
PINKB=\033[48;5;213m

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	@clear
	@printf "\033[0;32;3mLIBFT\t\t\033[0;32;0m✅\n"
	@sleep 0.2;
	@$(CC) $(CFLAGS) $(OBJS) $(OBJS_FAGS) $(MLX) $(LIBFT) -o $(NAME)
	@printf "\033[0;32;3mCubed\t\033[0;32;0m✅\033[0;32;3m\n\n"

$(OBJ_DIR)%.o: $(SRC_PATH)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_PATH)
	@mv $(LIBFT_PATH)/$(LIBFT) ./

$(MLX):
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4
	@mv $(MLX_PATH)/$(MLX) ./

clean:
	@rm -fr $(OBJ_DIR)
	@make clean -C $(LIBFT_PATH)

fclean:
	@rm -fr $(OBJ_DIR)
	@rm -f $(NAME)
	@rm -f $(MLX)
	@rm -fr $(MLX_PATH)
	@rm -f $(LIBFT)
	@make fclean -C $(LIBFT_PATH)
	
re: fclean all

.PHONY: all clean fclean re
