NAME = minishell
OBJDIR = obj
SOURCE =	split.c \
			minishell.c \
			split_helpers.c \
			rest_helpers.c \
			stepping_in_input.c \
			env_assign_helpers.c \
			syntax_error_check.c \
			environment_creation.c \
			dollar_sign_handler.c \
			skippers.c \
			is_quoted.c \
			quote_removal.c \
			special_characters_checkers.c \
			tokenizing.c \
			key_value_extraction.c \
			word_count_helpers.c \
			copying.c \
			path_handler.c \
			path_handler_helpers.c \
			cmd_list.c \
			cmd_list_helpers.c \
			redirections.c \
			heredoc.c \
			heredoc_helpers.c\
			executor.c \
			executor_helpers.c \
			executor_utils.c \
			built_in_executor.c \
			built_in_executor_helper.c \
			handling_expansions.c \
			expansion_helpers_1.c \
			expansion_helpers_2.c \
			expansion_helpers_3.c \
			expansion_helpers_4.c \
			count_letters.c \
			letter_checkers_1.c \
			letter_checkers_2.c \
			letter_checkers_3.c \
			preliminary_checkers.c \
			empty_string_and_quote_checker.c \
			signals.c \
			shlvl.c \
			signal_helpers.c \
			pwd.c \
			echo.c \
			envp.c \
			env.c \
			export.c \
			export_helpers.c \
			unset.c \
			exit.c \
			clean_ups.c \
			change_dir.c \
			general_helpers.c


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
