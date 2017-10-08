cmake_minimum_required( VERSION 3.0 )
project( AIBot C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )


SET( AIBOT_BUILDFILES 

	ChangeLog.txt
	CheckInstall.bat
	CleanAll.bat
	CMakeLists.txt
	Filters.cmake
	GenAIBootCampViewerFiles.bat
	GenAIBootCampViewerFiles.py
	INSTALL.txt
	PlayLocalMatch.bat
	rebuild_vis_sln_x64.bat
	SubmitAIBot.bat
	SubmitAIBot.py
	TeamInfo.team
)

SET( AIBOT_HEADERS 

	#Framework
	Bot.h	
	BotLogicIF.h
	Globals.h
	LevelInfo.h
	Logger.h
	NPCInfo.h
	ObjectInfo.h
	TileInfo.h
	TurnInfo.h

	#MY_BOT
	MyBotLogic/MyBotLogic.h
	MyBotLogic/Graph.h
	MyBotLogic/Node.h
	MyBotLogic/Agent.h
	MyBotLogic/Connector.h
	MyBotLogic/ComputeNode.h
	MyBotLogic/State.h
	MyBotLogic/Context.h
	
	MyBotLogic/State/WaitState.h
	MyBotLogic/State/MoveState.h
)

SET( AIBOT_SRC 

	#Framework
	Bot.cpp
	LevelInfo.cpp
	Logger.cpp
	TurnInfo.cpp

	#MY_BOT
	MyBotLogic/MyBotLogic.cpp
	MyBotLogic/Graph.cpp
	MyBotLogic/Node.cpp
	MyBotLogic/Agent.cpp
	MyBotLogic/Connector.cpp
	MyBotLogic/ComputeNode.cpp
	MyBotLogic/State.cpp
	MyBotLogic/Context.cpp
	
	MyBotLogic/State/WaitState.cpp
	MyBotLogic/State/MoveState.cpp
	
	#Main
	Main.cpp
)

SET(jobfiles "${AIBOT_HEADERS};${AIBOT_SRC};${AIBOT_BUILDFILES}")
file(WRITE jobfiles.txt "${jobfiles}")

add_executable(
	aibot

	${AIBOT_BUILDFILES}
	${AIBOT_HEADERS}
	${AIBOT_SRC}
)

CREATE_VISUAL_STUDIO_FILTERS(AIBOT)

