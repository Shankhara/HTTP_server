VPATH		= ../:../fds/
VPATH		+= ../:../parsing/
VPATH		+= ../:../responses/

NAME		= unittests
OBJS		= unittests.o Client.o Logger.o Server.o Utils.o Listener.o \
			Parsing.o Ut.o testParsing.o \
			CGIExec.o CGIResponse.o testCGI.o \
			Request.o testRequest.o \
			RespGet.o Response.o testResponse.o \


CXX			= clang++
CXXFLAGS	= -Wall -Wextra -Werror -g3
RM			= rm -f

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

unittests.o : unittests.hpp
testRequest.o: testRequest.cpp
testResponse.o: testResponse.cpp
testParsing.o: testParsing.cpp
testCGI.o: testCGI.cpp
Client.o : ../fds/Client.hpp
RespGet.o: ../responses/RespGet.hpp
Utils.o : ../Utils.hpp
Request.o : ../Request.hpp 
Logger.o : ../Logger.hpp
Server.o : ../Server.hpp
Listener.o : ../fds/Listener.hpp
Response.o : ../responses/Response.hpp
CGIExec.o : ../CGIExec.hpp
CGIResponse.o : ../fds/CGIResponse.hpp
Parsing.o : ../parsing/Parsing.hpp
Ut.o: ../parsing/Ut.hpp

clean:
			$(RM) $(OBJS)

fclean: 	clean
			$(RM) $(NAME)

re:			cgi fclean $(NAME)

tests: re
			./$(NAME)
			@$(RM) $(OBJS)

.PHONY: all clean fclean re
