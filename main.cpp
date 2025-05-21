//
// Created by alex on 18/05/25.
//

#include <iostream>
#include <string>

#include "src/Client/Client.hpp"

// int main(int argc, char** argv) {
//     string name;
//     if (argc != 2) {
//         name = "slabu";
//     }
//     else {
//         name = argv[1];
//     }
//     Client::renderer->init(argc, argv, name.c_str());
//     Entity a{"alex"};
//     Entity s("slabu");
//     glm::mat4 proj{1}, view{1};
//
//     Client::renderer->setDisplayFunc([&a, &s, &proj, &view]() {
//         Client::renderer->clear();
//         a.display(proj, view);
//         s.display(proj, view);
//         Client::renderer->swapBuffers();
//     });
//     Client::renderer->setReshapeFunc([&proj, &view](int w,int h) {
//         Client::renderer->viewport(0,0,w,h);
//         proj = glm::perspective<float>(M_PI/4, static_cast<float>(w)/static_cast<float>(h), 0.1f, 1000.0f);
//         view = glm::lookAt(vec3(10,10,10), vec3(0), vec3(0,1,0));
//     });
//
//     Client::renderer->setKeyboardFunc([&a](unsigned char key, int x, int y) {
//         if (key=='a') {
//             a.setRotation(vec3(250,250,0));
//         }
//         Client::renderer->redisplay();
//     });
//     Client::renderer->startMainLoop();
//
// }


int main(int argc, char** argv) {
    string name;
    if (argc != 2) {
        name = "slabu";
    }
    else {
        name = argv[1];
    }
    std::cout<<name<<std::endl;
    Client::renderer->init(argc, argv, name.c_str());
    auto& c { Client::getInstance(name)};

    c.run(argc, argv);

}


