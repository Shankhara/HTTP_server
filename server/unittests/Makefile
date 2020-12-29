VPATH		= ../
VPATH		+= ../fds/
VPATH		+= ../parsing/
VPATH		+= ../responses/

NAME		= unittests
OBJS		= unittests.o Client.o Logger.o Server.o Utils.o Listener.o \
			Parsing.o Ut.o CGIExec.o CGISocket.o autoIndex.o Mime.o Request.o Response.o \
			RespGet.o RespHead.o RespPut.o RespError.o RespPost.o RespDelete.o \
			testParsing.o testCGI.o testRequest.o testResponse.o \

CXX			= clang++
CXXFLAGS	= -Wall -Wextra -Werror -g3
RM			= rm -f

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

unittests.o : unittests.hpp
Client.o : ../fds/Client.hpp
autoIndex.o: ../responses/autoIndex.cpp
Utils.o : ../Utils.hpp
Request.o : ../Request.hpp 
Logger.o : ../Logger.hpp
Server.o : ../Server.hpp
Listener.o : ../fds/Listener.hpp
Response.o : ../responses/Response.hpp
CGIExec.o : ../CGIExec.hpp
CGISocket.o : ../fds/CGISocket.hpp
Parsing.o : ../parsing/Parsing.hpp
Ut.o: ../parsing/Ut.hpp
Mime.o : ../Mime.hpp
RespGet.o: ../responses/RespGet.hpp
RespPut.o: ../responses/RespPut.hpp
RespHead.o: ../responses/RespHead.hpp
RespError.o: ../responses/RespError.hpp
RespPost.o: ../responses/RespPost.hpp
RespDelete.o: ../responses/RespDelete.hpp
testRequest.o: testRequest.cpp
testResponse.o: testResponse.cpp
testParsing.o: testParsing.cpp
testCGI.o: testCGI.cpp

clean:
			$(RM) $(OBJS)

fclean: 	clean
			$(RM) $(NAME)

re:			cgi fclean $(NAME)

tests: re
			./$(NAME)
			@$(RM) $(OBJS)

.PHONY: all clean fclean re
