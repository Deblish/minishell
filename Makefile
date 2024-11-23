NAME = minishell
CC = cc
INCLUDES = -Iincludes
CFLAGS = -Wall -Wextra -Werror
HEADER = minishell.h

LIBFT = ./libft

SOURCES = srcs/parser/file.c \
		  srcs/parser/actions.c \
		  srcs/parser/enum_check.c \
		  srcs/parser/fetch_safe.c \
		  srcs/parser/parsing.c \
		  srcs/lexer/lexer.c \
		  srcs/lexer/token_operations.c \
		  srcs/lexer/tokenize_two.c \
		  srcs/lexer/tokenize.c \
		  srcs/lexer/tokens.c \
		  srcs/main.c

OBJECTS = $(SOURCES:.c=.o)

all: $(NAME)

$(LIBFT)/libft.a:
	make -C $(LIBFT)
	make bonus -C $(LIBFT)

$(NAME): $(OBJECTS) $(LIBFT)/libft.a
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT)/libft.a -o $(NAME)

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
