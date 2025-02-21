NAME = philo

SRC = src/init_data.c \
		src/parse.c \
		src/utils.c \
		src/dinner.c \
		src/philo.c

CC = cc

CC_FLAGS = -g -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CC_FLAGS) $(OBJ) -o $(NAME)
	@echo -n "Loading"
	@for i in {1..5}; do \
		echo -n "."; \
		sleep 0.5; \
	done
	@echo "✅"
	@echo "Files were generated successfully!✅✅✅"

.c.o:
	@$(CC) $(CC_FLAGS) -c $< -o $(<:.c=.o)

clean:
	@rm -f $(OBJ)
	@echo -n "Cleaning"
	@for i in {1..5}; do \
		echo -n "."; \
		sleep 0.5; \
	done
	@echo "✅"
	@echo "Files were deleted correctly!✅✅✅"

fclean: clean
	@rm -rf $(NAME)

re: fclean all

deadlock:
	@valgrind --tool=drd ./${NAME} ${args}

hellgrind:
	@valgrind --tool=helgrind ./${NAME} ${args}

v:
	@valgrind -s --track-origins=yes  --leak-check=full --show-leak-kinds=all ./${NAME} ${args}

c:
	@find . -type f -iname "*.c" -exec grep "//" {} +

.PHONY: all clean fclean re c