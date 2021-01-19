NAME	= webserv
SRCS	= main.cpp
SRCS	+= Utils.cpp
SRCS	+= Server.cpp
SRCS	+= Log.cpp
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
SRCS	+= responses/RespException.cpp
SRCS	+= responses/RespFile.cpp
SRCS	+= responses/RespFiles.cpp
SRCS	+= responses/Response.cpp
SRCS	+= responses/autoIndex.cpp
SRCS	+= responses/RespCGI.cpp
SRCS	+= fds/Configuration.cpp
SRCS	+= fds/Mime.cpp

HEADERS	= Utils.hpp
HEADERS	+= Server.hpp
HEADERS	+= Log.hpp
HEADERS	+= CGIExec.hpp
HEADERS	+= Request.hpp
HEADERS	+= fds/Listener.hpp
HEADERS	+= fds/Client.hpp
HEADERS	+= fds/CGISocket.hpp
HEADERS	+= parsing/Parsing.hpp
HEADERS	+= parsing/Ut.hpp
HEADERS	+= responses/RespGet.hpp
HEADERS	+= responses/RespHead.hpp
HEADERS	+= responses/RespPut.hpp
HEADERS	+= responses/RespPost.hpp
HEADERS	+= responses/RespDelete.hpp
HEADERS	+= responses/RespOptions.hpp
HEADERS	+= responses/RespTrace.hpp
HEADERS	+= responses/RespError.hpp
HEADERS	+= responses/RespException.hpp
HEADERS	+= responses/RespFile.hpp
HEADERS	+= responses/RespFiles.hpp
HEADERS	+= responses/Response.hpp
HEADERS	+= responses/RespCGI.hpp
HEADERS	+= fds/Configuration.hpp
HEADERS	+= fds/Mime.hpp

CFLAGS	= -Wall -Wextra -Werror -std=c++98
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
