# Box2D Physics Integration for Mario Game

This implementation replaces the previous QuadTree-based collision system with a complete Box2D physics engine integration, providing realistic physics simulation for the Mario game.

## Key Features

### 1. **Physics Manager (`PhysicsManager`)**
- Singleton pattern for global physics world management
- Fixed timestep physics simulation (60 FPS)
- Automatic cleanup and memory management
- Debug visualization support
- World boundaries creation

### 2. **Physics Bodies (`PhysicsBody`)**
- Wrapper around Box2D bodies that connects game objects to physics simulation
- Smart pointer management to prevent memory leaks
- Material system for different physics properties
- Position/velocity synchronization with game objects

### 3. **Contact Listener (`GameContactListener`)**
- Handles all collision events and responses
- Ground detection for character jumping mechanics
- Enemy collision behavior (bouncing, damage)
- Item collection detection
- Character vs enemy interactions (stomping, damage)

### 4. **Physics Factory (`PhysicsFactory`)**
- Factory methods for creating different types of physics bodies
- Predefined materials and settings for game objects
- Easy customization for different object types

### 5. **Material System (`PhysicsMaterial`)**
- Predefined materials for different object types:
  - `Ground()`: Static blocks with high friction
  - `Character()`: Dynamic bodies with moderate friction
  - `Enemy()`: Dynamic bodies with bounce
  - `Item()`: Sensor bodies (no collision response)
  - `Projectile()`: High restitution for bouncing
  - `Sensor()`: Trigger areas only

## Integration Points

### Character System
- Movement states work seamlessly with Box2D physics
- Velocity-based movement instead of position-based
- Ground detection through contact listener
- Jump mechanics use impulses instead of direct velocity setting

### Enemy System
- AI movement uses physics forces
- Collision response handled by Box2D
- Direction changes on wall collisions
- Gravity and ground detection automatic

### Block System
- Static physics bodies for collision
- No manual collision detection needed
- Automatic position synchronization

### Game Context
- Physics world initialization on game start
- Object spawning includes physics body creation
- Cleanup on state changes

## Physics Configuration

### World Settings
- Gravity: `(0, 980)` pixels/second² (Mario-style)
- Timestep: `1/60` seconds (60 FPS physics)
- Velocity iterations: `8`
- Position iterations: `3`

### Material Properties
```cpp
// Character
density: 1.0f, friction: 0.3f, restitution: 0.0f

// Ground/Blocks
density: 0.0f, friction: 0.7f, restitution: 0.0f

// Enemies
density: 1.0f, friction: 0.1f, restitution: 0.1f

// Items (Sensors)
density: 0.0f, friction: 0.0f, restitution: 0.0f, isSensor: true

// Projectiles
density: 0.3f, friction: 0.1f, restitution: 0.8f
```

## Usage Examples

### Creating a Character
```cpp
// Create game object
auto character = ObjectFactory::createCharacter(CharacterType::MARIO, {500, 500});

// Create physics body
BodyCreateInfo charInfo = PhysicsFactory::createCharacterBody({500, 500}, {32, 32});
PhysicsManager::getInstance().addObject(character, charInfo);
```

### Creating an Enemy
```cpp
// Create game object
auto enemy = ObjectFactory::createEnemy(EnemyType::GOOMBA, {100, 400}, {1, 1});

// Create physics body
BodyCreateInfo enemyInfo = PhysicsFactory::createEnemyBody({100, 400}, {32, 32}, EnemyType::GOOMBA);
PhysicsManager::getInstance().addObject(enemy, enemyInfo);
```

### Moving a Character
```cpp
// In character state (e.g., MovingState)
Vector2 currentVel = character->getVelocity();
character->setVelocity({speed, currentVel.y}); // Box2D automatically handles the physics
```

## Debug Features

### Visual Debug
- Enable with: `PhysicsManager::getInstance().setDebugDraw(true)`
- Shows physics body outlines
- Different colors for static (blue) vs dynamic (green) bodies
- Sleeping bodies shown in gray

### Debug Statistics
```cpp
int bodyCount, contactCount;
float frameTime;
PhysicsManager::getInstance().getDebugStats(bodyCount, contactCount, frameTime);
```

## Performance Optimizations

1. **Fixed Timestep**: Consistent physics simulation regardless of framerate
2. **Body Sleeping**: Inactive bodies automatically sleep to save CPU
3. **Contact Filtering**: Only processes relevant collision pairs
4. **Efficient Queries**: AABB queries for area-based object detection
5. **Smart Pointers**: Automatic memory management prevents leaks

## Collision Detection Improvements

### Old System Problems:
- Manual collision detection with QuadTree
- Inconsistent collision responses
- Frame-rate dependent physics
- Complex ground detection logic
- Manual velocity integration

### New System Benefits:
- Automatic collision detection and response
- Consistent physics simulation
- Frame-rate independent physics
- Realistic collision behaviors
- Professional physics engine reliability

## Migration Notes

### Removed Components:
- `QuadTree` class and related collision detection
- Manual collision response in `Character::handleEnvironmentCollision`
- Manual gravity application in `Character::applyGravity`
- Frame-based physics updates

### Modified Components:
- `PhysicsManager`: Complete rewrite using Box2D
- `Character`: Uses physics bodies for movement
- `Enemy`: Uses physics bodies for AI movement
- `GameContext`: Initializes physics world and creates bodies

### New Components:
- `PhysicsBody`: Box2D body wrapper
- `ContactListener`: Collision event handling
- `PhysicsFactory`: Body creation utilities
- `PhysicsMaterial`: Material property definitions

## Future Enhancements

1. **Particle Systems**: Use Box2D for particle physics
2. **Rope/Chain Physics**: Implement swinging mechanics
3. **Fluid Simulation**: Water areas with buoyancy
4. **Advanced Materials**: Ice (slippery), mud (slow), etc.
5. **Physics-based Puzzles**: Levers, switches, moving platforms
6. **Breakable Blocks**: Destruction system using Box2D joints

This Box2D integration provides a solid foundation for realistic physics simulation while maintaining the classic Mario game feel.
