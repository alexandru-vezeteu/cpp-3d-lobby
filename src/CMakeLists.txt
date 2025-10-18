cmake_minimum_required(VERSION 3.25)
project(Game)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

add_executable(Server
        server.cpp
        Socket/PosixSocket.cpp
        Socket/PosixSocket.hpp
        Server/Server.cpp
        Server/Server.hpp
        Network/Network.cpp
        Network/Network.hpp
        Config/Config.hpp
        Config/Config.cpp
        ./Command/Command.hpp
)

target_link_libraries(Server
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
)


if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Server
            ${CMAKE_THREAD_LIBS_INIT}
            X11
    )
endif()


add_executable(Client
        client.cpp
        OpenGL3DModel/OpenGL3DModel.cpp
        OpenGL3DModel/OpenGL3DModel.hpp
        Socket/PosixSocket.cpp
        Socket/PosixSocket.hpp
        Interfaces/IDraw.hpp
        Interfaces/IControl.hpp
        Entity/Entity.cpp
        Entity/Entity.hpp
        Client/Client.cpp
        Client/Client.hpp
        Player/Player.cpp
        Player/Player.hpp
        Network/Network.cpp
        Network/Network.hpp
        Config/Config.hpp
        ./Command/Command.hpp
        Threadsafe/ThreadSafeQueue.hpp
        Threadsafe/ThreadSafeUnorderedMap.hpp
        Interfaces/IRenderer.hpp
        GLUTRenderer/GLUTRenderer.cpp
        GLUTRenderer/GLUTRenderer.hpp
        Config/Config.cpp
        Compressor/Compressor.cpp
        Compressor/Compressor.hpp
)




target_link_libraries(Client
        OpenGL::GL
        GLUT::GLUT
        GLEW::GLEW
        assimp
)






if (UNIX AND NOT APPLE)
    find_package(Threads REQUIRED)
    target_link_libraries(Client
            ${CMAKE_THREAD_LIBS_INIT}
            X11
    )
endif()
