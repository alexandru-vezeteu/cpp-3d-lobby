cmake_minimum_required(VERSION 3.31)
project(Game)

set(CMAKE_CXX_STANDARD 23)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)  # Needed for modern OpenGL functions


set(CLIENT1_OUTPUT_DIR ${CMAKE_BINARY_DIR}/alex)
set(CLIENT2_OUTPUT_DIR ${CMAKE_BINARY_DIR}/slabu)

file(MAKE_DIRECTORY ${CLIENT1_OUTPUT_DIR})
file(MAKE_DIRECTORY ${CLIENT2_OUTPUT_DIR})

# Add executable
add_executable(Server
        server.cpp

        src/Socket/Socket.cpp
        src/Socket/Socket.hpp


        src/Server/Server.cpp
        src/Server/Server.hpp
        src/Network/Network.cpp
        src/Network/Network.hpp
        src/Config/Config.hpp
        src/Command/Command.hpp


)

add_executable(Client
        client.cpp
                src/OpenGL3DModel/OpenGL3DModel.cpp
                src/OpenGL3DModel/OpenGL3DModel.hpp
        src/Socket/Socket.cpp
        src/Socket/Socket.hpp
        src/Interfaces/IDrawable.hpp
        src/Interfaces/IControable.hpp
                src/Entity/Entity.cpp
                src/Entity/Entity.hpp
        src/Client/Client.cpp
        src/Client/Client.hpp
                src/Player/Player.cpp
                src/Player/Player.hpp
        src/Server/Server.cpp
        src/Server/Server.hpp
        src/Network/Network.cpp
        src/Network/Network.hpp
        src/Config/Config.hpp
        src/Command/Command.hpp
        src/Threadsafe/ThreadSafeQueue.hpp
        src/Threadsafe/ThreadSafeUnorderedMap.hpp
        src/Interfaces/IRenderer.hpp
        src/GLUTRenderer/GLUTRenderer.cpp
        src/GLUTRenderer/GLUTRenderer.hpp


)
set_target_properties(Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CLIENT1_OUTPUT_DIR})

add_executable(main
        main.cpp
                src/OpenGL3DModel/OpenGL3DModel.cpp
                src/OpenGL3DModel/OpenGL3DModel.hpp
        src/Socket/Socket.cpp
        src/Socket/Socket.hpp
        src/Interfaces/IDrawable.hpp
        src/Interfaces/IControable.hpp
        src/Interfaces/IRenderer.hpp
        src/Entity/Entity.cpp
                src/Entity/Entity.hpp
        src/Client/Client.cpp
        src/Client/Client.hpp
                src/Player/Player.cpp
                src/Player/Player.hpp
        src/Server/Server.cpp
        src/Server/Server.hpp
        src/Network/Network.cpp
        src/Network/Network.hpp
        src/Config/Config.hpp
        src/Command/Command.hpp
        src/Threadsafe/ThreadSafeQueue.hpp
        src/Threadsafe/ThreadSafeUnorderedMap.hpp
        src/GLUTRenderer/GLUTRenderer.cpp
        src/GLUTRenderer/GLUTRenderer.hpp


)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CLIENT2_OUTPUT_DIR})


# Link libraries
target_link_libraries(Server
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
)

target_link_libraries(Client
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
)

target_link_libraries(main
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
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
