# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 14:32:39 by alassiqu          #+#    #+#              #
#    Updated: 2024/06/14 13:04:59 by alassiqu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= minishell

LIBFT_PATH		:= libft/
LIBFT			:= libft/libft.a

CC				:= cc
CFLAGS			:= -Wall -Werror -Wextra -fsanitize=address -g3
RM				:= rm -f

BUILTINS		:=	builtins/ft_env.c 					\
# 					builtins/ft_cd.c 					\
# 					builtins/ft_echo.c 					\
# 					builtins/ft_env_utils.c 			\
# 					builtins/ft_exit.c 					\
# 					builtins/ft_export.c 				\
# 					builtins/ft_pwd.c 					\
# 					builtins/ft_unset.c

CLEANING		:=	cleaning/clean.c

ENVIRONMENT		:=	environment/env_utils.c				\
					environment/free.c

EXPANDING		:=	expanding/expander.c				\
					expanding/word_helper.c				\
					expanding/post_expand.c				\
					expanding/dollar_expand.c			\
					expanding/quotes_expand.c

MEMORY			:=	memory/memory.c						\
					memory/garbage_collector.c

NODES			:=	nodes/node.c

PARSING			:=	parsing/parse.c

SYNTAXING		:=	syntax/syntax.c						\
					syntax/syntax_utils.c				\
					syntax/pipe_or_and.c				\
					syntax/redirection.c				\
					syntax/parenthesis_utils.c			\
					syntax/parenthesis.c

SIGNALS			:=	signals/handle_signals.c


TOKENIZATION	:=	tokenization/tokenizer.c			\
					tokenization/tokenizer_utils.c		\
					tokenization/tokenizer_helpers.c	\
					tokenization/tokenizer_free.c


SRCS			:=	$(BUILTINS)							\
					$(CLEANING)							\
					$(ENVIRONMENT)						\
					$(EXPANDING)						\
					$(MEMORY)							\
					$(NODES)							\
					$(PARSING)							\
					$(SIGNALS)							\
					$(SYNTAXING)						\
					$(TOKENIZATION)						\
					minishell.c

OBJS			:=	$(SRCS:.c=.o)

all:	$(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)
	make -C $(LIBFT_PATH) bonus

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_PATH) -lft -lreadline -o $(NAME) 

clean:
	@make clean -C $(LIBFT_PATH)
	@make clean -C $(LIBFT_PATH)
	@rm -f $(OBJS)

fclean: clean
	@make fclean -C $(LIBFT_PATH)
	@rm -f $(NAME)

re: fclean all

.PHONY: clean