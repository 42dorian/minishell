NAME = minishell
OBJDIR = obj
SOURCE =	split.c \
			minishell.c \
			split_helpers.c \
			rest_helpers.c \
			expansion_check.c \
			stepping_in_input.c \
			env_assign_helpers.c \
			syntax_error_check.c \
			environment_creation.c \
			dollar_sign_handler.c \
			is_special_character.c \
			skippers.c \
			special_characters_checkers.c \
			tokenizing.c \
			word_count_helpers.c \
			path_handler.c \
			path_handler_helpers.c \
			cmd_list.c \
			cmd_list_helpers.c \
			redirections.c \
			executor.c \
			executor_helpers.c \
			built_in_executor.c \
			built_in_executor_helper.c \
			signals.c \
			pwd.c \
			echo.c \
			envp.c \
			env.c \
			export.c \
			unset.c \
			exit.c \
			change_dir.c


# CFLAGS = -Wall -Wextra -Werror -g
CFLAGS = -I/opt/homebrew/opt/readline/include -g
READLINE_FLAG = -L/opt/homebrew/opt/readline/lib -lreadline


CC = cc

LIBFT_DIR = libft
LIBFT = ${LIBFT_DIR}/libft.a

OBJECTS = $(SOURCE:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME) : $(OBJECTS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) -o $(NAME) $(READLINE_FLAG)

clean:
	@make -C $(LIBFT_DIR) clean
	rm -rf $(OBJDIR)

fclean:
	@make -C $(LIBFT_DIR) fclean
	rm -rf $(OBJDIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
