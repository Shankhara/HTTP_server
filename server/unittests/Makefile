VPATH		= ../:../fds/

NAME		= unittests
OBJS		= unittests.o Client.o Request.o Logger.o Server.o Response.o CGIExec.o CGIResponse.o

CXX			= clang++
CXXFLAGS	= -Wall -Wextra -Werror #-std=c++98
RM			= rm -f

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

unittests.o : unittests.hpp
Client.o : ../fds/Client.hpp
Request.o : ../Request.hpp 
Logger.o : ../Logger.hpp
Server.o : ../Server.hpp
Response.o : ../Response.hpp
CGIExec.o : ../CGIExec.hpp
CGIResponse.o : ../fds/CGIResponse.hpp

clean:
			$(RM) $(OBJS)

fclean: 	clean
			$(RM) $(NAME)

re:			fclean $(NAME)

.PHONY: all clean fclean re
