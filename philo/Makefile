# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mevangel <mevangel@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/28 16:25:24 by mevangel          #+#    #+#              #
#    Updated: 2024/02/09 20:22:30 by mevangel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread #-fsanitize=thread -g

GREEN	=	\033[0;32m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

SRC_DIR	=	./
OBJ_DIR	=	./objs/

SRC		=	$(SRC_DIR)main.c \
			$(SRC_DIR)philo_routine.c \
			$(SRC_DIR)supervise.c \
			$(SRC_DIR)utils.c

OBJ		=	$(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@echo "$(GREEN)philo compiled!$(CLEAR)"

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
		@echo "$(CYAN)Philo executable and object files cleaned!$(CLEAR)"

re: fclean all

.PHONY: all clean fclean re