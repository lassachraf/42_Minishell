# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alassiqu <alassiqu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/04 14:32:39 by alassiqu          #+#    #+#              #
#    Updated: 2024/08/06 17:34:24 by alassiqu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:=	minishell

LIBFT_PATH		:=	libft/
LIBFT			:=	libft/libft.a
# -Werror
CC				:=	cc
CFLAGS			:=	-Wall -Wextra -Werror #-fsanitize=address -g3
RM				:=	rm -f

BUILTINS		:=	builtins/builtins_helpers.c			\
					builtins/builtins_utils.c 			\
					builtins/exit_utils.c				\
					builtins/export_handlers.c			\
					builtins/export_utils.c				\
					builtins/ft_cd.c 					\
					builtins/ft_echo.c 					\
					builtins/ft_env.c 					\
					builtins/ft_exit.c 					\
					builtins/ft_export.c 				\
					builtins/ft_pwd.c 					\
					builtins/ft_unset.c					\
					builtins/setters.c

ENVIRONMENT		:=	environment/env_helpers.c			\
					environment/env_utils.c

EXECUTING		:=	executing/convert_env.c				\
					executing/executer.c				\
					executing/execve_tools.c			\
					executing/here_doc_tools.c			\
					executing/here_doc_utils.c			\
					executing/here_doc.c				\
					executing/redir_tools.c				\
					executing/tools_1.c					\
					executing/tools_2.c					\
					executing/tools.c					\
					executing/executer_tools.c

EXPANDING		:=	expanding/dollar_expand.c			\
					expanding/expand_here_doc.c			\
					expanding/expander_utils.c			\
					expanding/expander.c				\
					expanding/fill_expand.c				\
					expanding/wildcard_expand.c			\
					expanding/word_helper.c

MEMORY			:=	memory/garbage_collector.c			\
					memory/memory.c

PARSING			:=	parsing/parse.c						\
					parsing/node.c						\
					parsing/parse_tools.c				\
					parsing/parse_tools2.c

SIGNALS			:=	signals/handle_signals.c

SYNTAXING		:=	syntax/parenthesis_utils.c			\
					syntax/parenthesis.c				\
					syntax/pipe_or_and.c				\
					syntax/redirection.c				\
					syntax/syntax_utils.c				\
					syntax/syntax.c

TOKENIZING		:=	tokenizing/tokenizer_free.c			\
					tokenizing/tokenizer_helpers.c		\
					tokenizing/tokenizer_utils.c		\
					tokenizing/tokenizer.c				\
					tokenizing/tokens_helpers.c

MAIN			:=	main_utils.c						\
					minishell.c

SRCS			:=	$(BUILTINS)							\
					$(CLEANING)							\
					$(ENVIRONMENT)						\
					$(EXECUTING)						\
					$(EXPANDING)						\
					$(MEMORY)							\
					$(PARSING)							\
					$(SIGNALS)							\
					$(SYNTAXING)						\
					$(TOKENIZING)						\
					$(MAIN)

OBJS			:=	$(SRCS:.c=.o)

all:	$(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)
	make -C $(LIBFT_PATH) bonus

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_PATH) -lft -lreadline -o $(NAME)

clean:
	@make clean -C $(LIBFT_PATH)
	@rm -f $(OBJS)

fclean: clean
	@make fclean -C $(LIBFT_PATH)
	@rm -f $(NAME)

re: fclean all

.PHONY: clean
