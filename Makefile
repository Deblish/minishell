NAME = minishell
CC = cc
INCLUDES = -Iincludes -I/opt/homebrew/opt/readline/include
CFLAGS = -Wall -Wextra -Werror
HEADER = minishell.h

LIBFT = ./libft

SOURCES = srcs/lexer/stack/token_utils.c \
		  srcs/lexer/stack/token_operations.c \
		  srcs/lexer/tokenize.c \
		  srcs/lexer/lexer.c \
		  srcs/parser/stack/stack_utils.c \
		  srcs/parser/stack/stack_operations.c \
		  srcs/parser/file.c \
		  srcs/parser/grammar_utils.c \
		  srcs/parser/actions.c \
		  srcs/parser/enum_check.c \
		  srcs/parser/parsing.c \
		  srcs/parser/ast.c \
		  srcs/execution/simple_cmd.c \
		  srcs/execution/pipe_sequence.c \
		  srcs/execution/execute.c \
		  srcs/info.c \
		  srcs/main.c \
		  srcs/signals/signals.c \
		  srcs/env/env.c \
		  srcs/env/env_utils.c \
		  srcs/builtins/cd.c \
		  srcs/builtins/cd_utils.c \
		  srcs/builtins/pwd.c \
		  srcs/builtins/echo.c \
		  srcs/builtins/ft_env.c \
		  srcs/builtins/export.c \
		  srcs/builtins/export_utils.c \
		  srcs/builtins/export_utilstwo.c \
		  srcs/builtins/unset.c \
		  srcs/builtins/ft_exit.c \

OBJECTS = $(SOURCES:.c=.o)

READLINE = -lreadline -L/opt/homebrew/opt/readline/lib

all: $(NAME)

$(LIBFT)/libft.a:
	make -C $(LIBFT)
	make bonus -C $(LIBFT)

$(NAME): $(OBJECTS) $(LIBFT)/libft.a
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT)/libft.a $(READLINE) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(OBJECTS)
	make -C $(LIBFT) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re

