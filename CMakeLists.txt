cmake_minimum_required(VERSION 3.23)
project(pjc_hexagon)

set(CMAKE_CXX_STANDARD 20)

add_executable(pjc_hexagon src/main.cpp src/UI/UI.h src/UI/ConsoleUI.cpp src/UI/ConsoleUI.h src/Game/Game.cpp src/Game/Game.h src/Game/Teams.cpp src/Game/Teams.h src/Game/Team.cpp src/Game/Team.h src/Game/Board.cpp src/Game/Board.h src/Game/Field.cpp src/Game/Field.h src/Game/Move.cpp src/Game/Move.h src/Consts.h src/Game/Points.cpp src/Game/Points.h src/FileManagement/GameSerializer.cpp src/FileManagement/GameSerializer.h src/FileManagement/FileManager.cpp src/FileManagement/FileManager.h)
