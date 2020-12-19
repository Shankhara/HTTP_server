VPATH		= ../:../fds/

NAME		= unittests
OBJS		= unittests.o Client.o Request.o Logger.o Server.o Response.o CGIExec.o CGIResponse.o Utils.o testRequest.o testCGI.o Listener.o

CXX			= clang++
CXXFLAGS	= -Wall -Wextra -Werror -g3
RM			= rm -f

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

unittests.o : unittests.hpp
testRequest.o: testRequest.cpp
testCGI.o: testCGI.cpp
Client.o : ../fds/Client.hpp
Utils.o : ../Utils.hpp
Request.o : ../Request.hpp 
Logger.o : ../Logger.hpp
Server.o : ../Server.hpp
Listener.o : ../fds/Listener.hpp
Response.o : ../Response.hpp
CGIExec.o : ../CGIExec.hpp
CGIResponse.o : ../fds/CGIResponse.hpp

clean:
			$(RM) $(OBJS)

fclean: 	clean
			$(RM) $(NAME)

re:			fclean $(NAME)

tests: re
			./$(NAME)

.PHONY: all clean fclean re
