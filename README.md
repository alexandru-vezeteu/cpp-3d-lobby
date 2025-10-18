# 3D Multiplayer Prototype

A networked 3D multiplayer application demonstrating real-time entity synchronization, OpenGL rendering, and client-server architecture. Built as a technical showcase for concurrent systems, network programming, and graphics development.

## Overview

This prototype implements a basic multiplayer 3D environment where players can join a shared world, move around, and see other players in real-time. The client renders 3D models using OpenGL/GLUT while maintaining continuous network synchronization with a central server that manages game state.

## Architecture


![Class Diagram](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.githubusercontent.com/alexandru-vezeteu/cpp-3d-lobby/main/doc/uml.puml)
UML diagram

### Client-Server Model
The system uses a traditional authoritative server architecture:
- **Server**: Manages canonical player state, broadcasts updates to all clients, and handles join/leave events
- **Client**: Renders the 3D scene, processes user input, sends state changes, and applies updates from other players

### Key Components

#### Networking Layer
- **Socket Interface**: Abstracted socket operations with POSIX implementation for cross-platform potential
- **Custom Protocol**: Binary protocol for efficient command serialization (JOIN, LEAVE, UPDATE)
- **Network Class**: High-level networking operations including file transfer, vector serialization, and command handling

#### Rendering System
- **Interface-Based Design**: `IRenderer` interface allows swapping rendering backends (currently GLUT)
- **Entity System**: Entity base class implementing `IDraw` interface for polymorphic rendering
- **OpenGL Pipeline**: Custom 3D model loader supporting meshes with diffuse, normal, and roughness textures
- **Camera System**: First-person camera attached to player entity with configurable offset

#### Concurrency
- **Thread-Safe Collections**: Custom `ThreadSafeQueue` and `ThreadSafeUnorderedMap` using shared mutexes
- **Command Queue Pattern**: Decouples network I/O from game logic updates
- **Per-Client Threading**: Each connected client handled by dedicated server thread

## Design Decisions

**Interface Segregation**: Separated `IDraw`, `IControl`, and `IRenderer` interfaces to allow components to implement only needed functionality. Players implement both `IDraw` and `IControl`, while entities only implement `IDraw`.

**Move Semantics**: Heavy use of move constructors/operators and deleted copy constructors to prevent expensive copies of GPU resources and network objects.

**Template Collections**: Generic thread-safe containers reduce code duplication while maintaining type safety for different command and state types.

**Renderer Abstraction**: GLUT wrapped behind `IRenderer` interface to enable future migration to modern rendering APIs (Vulkan, DirectX) without touching game logic.

**Smart Pointers**: Used `unique_ptr` for owned resources and `shared_ptr` in thread-safe map to manage lifetime across threads safely.

## Deployment

Both client and server have been containerized using Docker for simplified deployment and dependency management. This eliminates environment-specific build issues and ensures consistent behavior across platforms.

```bash
# Run server from the root of the project
./runServer [IP] [port]
# by default IP = 0.0.0.0 and port = 2898

# Run client
./runClient name [IP] [port]
# name is necessary and by default it looks into the players_example folder and gets the folder with the name
# by default IP = 0.0.0.0 and port = 2898
# for more details check the dockerfiles and the contents of the scripts
```

## Known Weaknesses & Solutions

### 1. **No Authoritative Movement Validation**
**Problem**: Clients send position updates directly; server blindly broadcasts them. Enables cheating and desync issues.

**Solution**: Implement server-side physics simulation. Clients send input commands, server computes positions, broadcasts authoritative state. Add client-side prediction with reconciliation for smooth movement.

### 2. **Unencrypted Communication**
**Problem**: All network traffic sent in plaintext, exposing player data and enabling packet manipulation.

**Solution**: Implement TLS/DTLS for socket communication or add application-layer encryption (e.g., libsodium). Include HMAC for message authentication.

### 3. **Blocking Network Operations**
**Problem**: `RecvAll`/`SendAll` block threads. Under heavy load or slow networks, could cause stuttering or thread starvation.

**Solution**: Migrate to asynchronous I/O (epoll, io_uring, or Boost.Asio). Implement non-blocking sockets with event-driven architecture.

### 4. **GLUT Renderer Limitations**
**Problem**: GLUT is deprecated, single-threaded, and lacks modern rendering features (PBR, compute shaders, advanced lighting).

**Solution**: Replace with modern graphics API. Implement `IRenderer` for SDL2 + OpenGL 4.5+, or migrate to Vulkan for explicit control and better performance.

### 5. **Fixed Function Pipeline**
**Problem**: Shader compilation happens per-model. No shader management system or material system for shared resources.

**Solution**: Implement shader cache and material system. Create render queue sorted by material/shader to minimize state changes. Add instanced rendering for repeated entities.

### 6. **No State Interpolation**
**Problem**: Network updates arrive at discrete intervals. Players "teleport" between positions instead of smooth movement.

**Solution**: Implement client-side interpolation between received states. Buffer recent positions and interpolate rendering position while waiting for next update.

### 7. **Naive Collision Detection**
**Problem**: No collision system. Players can move through each other and world geometry.

**Solution**: Add spatial partitioning (octree/BVH) for broad phase. Implement swept collision detection for continuous collision. Consider physics engine integration (Bullet, PhysX).

### 8. **Unbounded Resource Growth**
**Problem**: No limits on connected clients. `ThreadSafeUnorderedMap` and queues can grow indefinitely, leading to memory exhaustion.

**Solution**: Add connection limits and resource quotas. Implement command queue size limits with backpressure. Add automatic cleanup of disconnected client resources.

### 9. **Single Server Bottleneck**
**Problem**: All clients connect to one server. Doesn't scale beyond ~100 concurrent players depending on hardware.

**Solution**: Implement spatial partitioning with multiple server instances. Use interest management to only sync nearby players. Consider dedicated server architecture with load balancing.

## Custom Content Support

### Player Model & Shader Upload
Players can upload custom 3D models and GLSL shaders to personalize their in-game appearance. The network layer includes file transfer capabilities that sync custom assets from client to server and distribute them to other connected clients.

### PBR Shader Implementation
The default shader implements physically-based rendering (PBR) using the Cook-Torrance BRDF model:

**Vertex Shader**:
- Constructs TBN (Tangent-Bitangent-Normal) matrix for tangent-space normal mapping
- Transforms positions through MVP matrices for correct projection
- Passes interpolated world position and texture coordinates to fragment shader

**Fragment Shader**:
- **Material System**: Supports three texture maps:
  - Diffuse/Albedo for base color
  - Normal map for surface detail without geometry
  - Combined metallic-roughness-AO map (R=AO, G=roughness, B=metallic)
- **Lighting Model**: Multiple point lights with physically accurate attenuation (inverse square law)
- **PBR Components**:
  - **Fresnel-Schlick**: Accurate reflection behavior at grazing angles
  - **GGX Normal Distribution**: Microfacet distribution for specular highlights
  - **Smith Geometry Function**: Self-shadowing and masking of microfacets
- **Tone Mapping**: Reinhard operator with gamma correction for HDR to LDR conversion
- **Energy Conservation**: Diffuse and specular balance based on metallic property

The shader supports dynamic light configurations (currently 4 lights) and properly handles the metallic workflow where metallic surfaces have no diffuse component and colored specular reflections.

## Technical Highlights

- **Concurrent Command Processing**: Lock-free queue operations where possible, minimizing contention
- **Resource Management**: RAII patterns ensure proper cleanup of OpenGL objects and network sockets
- **Extensible Entity System**: Easy to add new entity types by inheriting from base classes
- **Model Loading**: Assimp integration for loading multiple 3D formats (OBJ, FBX, GLTF)
- **Advanced Rendering**: PBR shading with normal mapping and physically accurate lighting
- **Custom Content Pipeline**: File transfer protocol for sharing models and shaders between clients

## Build Requirements

- C++17 or later
- OpenGL 3.3+
- GLUT/FreeGLUT
- Assimp
- GLM (OpenGL Mathematics)
- POSIX-compliant system (Linux/macOS)

## Future Enhancements

- Physics simulation (Bullet/PhysX integration)
- Scripting system (Lua/Python bindings)
- Advanced lighting (PBR, shadow mapping)
- Asset streaming
- Persistence layer (player data, world state)
- Web-based admin interface

---

*This is a prototype demonstrating architectural patterns and technical capabilities. Not intended for production use.*