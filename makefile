CC = g++
FLAGS = -std=c++14 -Wall -g
TARGET = main
OBJS  = main.o Compresser.o
OBJSDIR  = .obj

$(TARGET):$(OBJS)
	$(CC) $(OBJSDIR)/*.o -o $(TARGET) $(FLAGS)

$(OBJS):%.o:%.cpp
	$(CC) -c $< -o $(OBJSDIR)/$@ $(FLAGS)

clean:
	rm *.comp *.huff *.dpred
	