
NAME		=	mod1
CXX			=	c++
FLAGS		=	-Wall -Wextra -Werror -g3
GLFLAGS		=	-lglfw -lGL -lGLU -lX11 -lm -pthread

SRCS		=	./src/main.cpp		\
				./src/WaterSurface.class.cpp \
				./src/Cell.class.cpp \
				./src/Renderer2D.class.cpp \

HEADERS		=	./include

OBJS		=	$(SRCS:.cpp=.o)

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
