
NAME		=	mod1
CXX			=	c++
FLAGS		=	-Wall -Wextra -Werror -g3
GLFLAGS		=	-lglfw -lGL -lGLU -lX11 -lm -pthread

SRCS		=	./src/main.cpp		\
				./src/Simulation.class.cpp \
				./src/MapGenerator.class.cpp \
				./src/WaterSurface.class.cpp \
				./src/Cell.class.cpp		\
				./src/Renderer.class.cpp	\
				./src/Camera.class.cpp


HEADERS		=	./include

OBJS		=	$(SRCS:.cpp=.o)


GLEW_CHECK := $(shell brew list glew --versions 2>/dev/null)


all:	$(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(FLAGS) $(OBJS) $(GLFLAGS) -o $(NAME)
	@echo "Compiling $(NAME)"

%.o: %.cpp
	@$(CXX) $(FLAGS) -I $(HEADERS) -c  $< -o $@
	@echo "Compiling $<"

clean:
	@rm -rf $(OBJS)
	@echo "Cleaning..."

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re
