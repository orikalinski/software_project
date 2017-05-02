CC = gcc
C++ = g++
OBJS = main.o main_aux.o SPImageProc.o SPPoint.o SPBPriorityQueue.o SPConfig.o SPLogger.o SPKDTree.o SPKDArray.o sp_utils.o
EXEC = fp
INCLUDEPATH=/usr/local/lib/opencv-3.1.0/include/
LIBPATH=/usr/local/lib/opencv-3.1.0/lib/
LIBS=-lopencv_xfeatures2d -lopencv_features2d \
-lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core


CPP_COMP_FLAG = -std=c++11 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG -g

C_COMP_FLAG = -std=gnu99 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG -g

$(EXEC): $(OBJS)
	$(C++) $(OBJS) -L$(LIBPATH) $(LIBS) -o $@
main.o: main.cpp main_aux.h SPImageProc.h SPPoint.h SPBPriorityQueue.h SPKDTree.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
main_aux.o: main_aux.h main_aux.cpp SPPoint.h SPBPriorityQueue.h SPKDTree.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPImageProc.o: SPLogger.h SPLogger.c SPImageProc.h SPImageProc.cpp SPPoint.h SPBPriorityQueue.h SPKDTree.h
	$(C++) $(CPP_COMP_FLAG) -I$(INCLUDEPATH) -c $*.cpp
SPPoint.o: SPPoint.c SPPoint.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPConfig.o: SPConfig.c SPConfig.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPKDArray.o: SPKDArray.c SPKDArray.h
	$(CC) $(C_COMP_FLAG) -c $*.c
SPKDTree.o: SPKDTree.c SPKDTree.h
	$(CC) $(C_COMP_FLAG) -c $*.c
sp_utils.o: sp_utils.c sp_utils.h
	$(CC) $(C_COMP_FLAG) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)
