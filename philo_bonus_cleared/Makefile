# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 16:25:24 by mevangel          #+#    #+#              #
#    Updated: 2024/02/16 10:37:28 by mevangel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

GREEN	=	\033[0;32m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

SRC_DIR	=	./
OBJ_DIR	=	./objs/

SRC		=	$(SRC_DIR)main_bonus.c \
			$(SRC_DIR)routine_bonus.c \
			$(SRC_DIR)utils_bonus.c \
			$(SRC_DIR)helper_functions_bonus.c

OBJ		=	$(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(GREEN)philo_bonus compiled!$(CLEAR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(CYAN)object files cleaned!$(CLEAR)"

fclean:
		@rm -rf $(OBJ_DIR)
		@rm -f $(NAME)
		@echo "$(CYAN)Philo_bonus executable and object files cleaned!$(CLEAR)"

re: fclean all

.PHONY: all clean fclean re