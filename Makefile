
NAME		=	mod1
CXX			=	c++
FLAGS		=	-Wall -Wextra -Werror -g3
# GLFLAGS		=	-lglfw -lGL -lGLU -lX11 -lm -pthread -lGLEW 
GLFLAGS		= -L ./lib/glew -lGLEW -lglfw -lGL -lGLU -lX11 -lm -pthread

SRCS		=	./src/main.cpp		\
				./src/Simulation.class.cpp \
				./src/MapGenerator.class.cpp \
				./src/WaterSurface.class.cpp \
				./src/Cell.class.cpp		\
				./src/Renderer.class.cpp	\
				./src/Camera.class.cpp


HEADERS		=	-I ./include

LIBS		=	-I ./lib/glew -I ./lib/glm -I ./lib/

OBJS		=	$(SRCS:.cpp=.o)



all:	$(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(FLAGS) $(OBJS) $(GLFLAGS) -o $(NAME)
	@echo "Compiling $(NAME)"

%.o: %.cpp
	@$(CXX) $(FLAGS) $(HEADERS) $(LIBS) -c  $< -o $@
	@echo "Compiling $<"

clean:
	@rm -rf $(OBJS)
	@echo "Cleaning..."

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY : all clean fclean re
