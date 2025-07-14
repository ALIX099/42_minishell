CC=cc
CFLAGS=-Wall -Wextra -Werror -g3 -Iheaders/
SRCS=$(wildcard *.c parser/*.c parser/*/*.c expander/*.c executor/*.c)
ARCHIVE=libft/libft.a
TARGET=minishell

$(TARGET): $(SRCS) $(ARCHIVE)
	$(CC) $(CFLAGS) -lreadline $^ -o $@
	@echo "Compilation complete. Run './$(TARGET)' to start the shell."

$(ARCHIVE):
	@make -C libft
	@echo "Libft compiled successfully."

clean:
	@make -C libft clean
	@echo "Cleaned up the project."

fclean: clean
	@make -C libft fclean
	@rm -f $(TARGET)
	@echo "Fully cleaned the project."

re: fclean
	@make
	@echo "Recompiled the project."

.PHONY: all clean fclean re