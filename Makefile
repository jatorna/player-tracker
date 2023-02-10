LIBS = -lboost_program_options -lboost_log -lboost_log_setup -lboost_thread -lboost_system -lboost_thread -lpthread -lopencv_videoio -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_video

# Nombre del ejecutable
EXE = player-tracker

# Nombre de los archivos fuente
SRCS = src/AlgorithmManager.cpp src/AlgorithmManager.hpp src/ComputerVisionEngine.cpp src/ComputerVisionEngine.hpp src/KalmanFilter.cpp src/KalmanFilter.hpp src/LowLevelFunctions.cpp src/LowLevelFunctions.hpp src/TrackingEngine.cpp src/TrackingEngine.hpp src/Types.h src/player-tracker.cpp

# Reglas del makefile
all: $(EXE)

$(EXE): $(SRCS)
	mkdir -p build
	g++ -O2 -g -Wall -fmessage-length=0 -o build/$(EXE) $(SRCS) $(LIBS)

clean:
	rm -f build/$(EXE)
