NAME	= webserv
SRCS	= main.cpp
SRCS	+= Utils.cpp
SRCS	+= Server.cpp
SRCS	+= Logger.cpp
SRCS	+= CGIExec.cpp
SRCS	+= Request.cpp
SRCS	+= fds/Listener.cpp
SRCS	+= fds/Client.cpp
SRCS	+= fds/CGISocket.cpp
SRCS	+= parsing/Parsing.cpp
SRCS	+= parsing/Ut.cpp
SRCS	+= responses/RespGet.cpp
SRCS	+= responses/RespHead.cpp
SRCS	+= responses/RespPut.cpp
SRCS	+= responses/RespPost.cpp
SRCS	+= responses/RespDelete.cpp
SRCS	+= responses/RespOptions.cpp
SRCS	+= responses/RespTrace.cpp
SRCS	+= responses/RespError.cpp
SRCS	+= responses/Response.cpp
SRCS	+= responses/autoIndex.cpp
SRCS	+= Mime.cpp

CFLAGS	= -Wall -Wextra -Werror -g3 -std=c++98
CC		= clang++

OBJDIR	= .objs
OBJS	=	$(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

$(NAME): $(OBJS)
		${CC} ${CFLAGS} $(OBJS) -o $(NAME)

all:		$(NAME)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
		@mkdir $(OBJDIR)
		@mkdir -p $(OBJDIR)/fds
		@mkdir -p $(OBJDIR)/parsing
		@mkdir -p $(OBJDIR)/responses

${OBJDIR}/%.o: %.cpp
		${CC} ${CFLAGS} -c $< -o $@

clean:
			rm -rf $(OBJDIR)

fclean:	clean
			rm -f $(NAME)

re:		fclean all

run: re
	@./webserv
	@rm -f ./webserv

leaks: re
	@valgrind --track-fds=yes --leak-check=full --show-leak-kinds=all ./webserv -v
	@rm -f ./webserv

tests:
	@make -C ./unittests/
	@./unittests/unittests

verbose: re
	@ ./webserv -v
	@rm -f ./webserv
	


.PHONY: all clean fclean re run tests
