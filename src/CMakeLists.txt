cmake_minimum_required(VERSION 3.25)
project(Game)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

add_executable(Server
        server.cpp
        src/Socket/PosixSocket.cpp
        src/Socket/PosixSocket.hpp
        src/Server/Server.cpp
        src/Server/Server.hpp
        src/Network/Network.cpp
        src/Network/Network.hpp
        src/Config/Config.hpp
        src/Config/Config.cpp
        src/Command/Command.hpp
)

add_executable(Client
        client.cpp
        src/OpenGL3DModel/OpenGL3DModel.cpp
        src/OpenGL3DModel/OpenGL3DModel.hpp
        src/Socket/PosixSocket.cpp
        src/Socket/PosixSocket.hpp
        src/Interfaces/IDraw.hpp
        src/Interfaces/IControl.hpp
        src/Entity/Entity.cpp
        src/Entity/Entity.hpp
        src/Client/Client.cpp
        src/Client/Client.hpp
        src/Player/Player.cpp
        src/Player/Player.hpp
        src/Network/Network.cpp
        src/Network/Network.hpp
        src/Config/Config.hpp
        src/Command/Command.hpp
        src/Threadsafe/ThreadSafeQueue.hpp
        src/Threadsafe/ThreadSafeUnorderedMap.hpp
        src/Interfaces/IRenderer.hpp
        src/GLUTRenderer/GLUTRenderer.cpp
        src/GLUTRenderer/GLUTRenderer.hpp
        src/Config/Config.cpp
        src/Compressor/Compressor.cpp
        src/Compressor/Compressor.hpp
)



target_link_libraries(Server
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
)

target_link_libraries(Client
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
        assimp
)



# For Linux, explicitly link X11 and other dependencies
if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Server
            ${CMAKE_THREAD_LIBS_INIT}
            X11
    )
endif()


if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Client
            ${CMAKE_THREAD_LIBS_INIT}
            X11
    )
endif()
