cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_TowerDefense)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(Resource)
include_directories(${CMAKE_BINARY_DIR}/generated)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

execute_process(
    COMMAND git log -1 --pretty=%s
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file(
    ${CMAKE_SOURCE_DIR}/Version.hpp.in
    ${CMAKE_BINARY_DIR}/generated/Version.hpp      # output location
    @ONLY)                                         # only replace tokens

add_executable(2025_I2P2_TowerDefense
    Bullet/Bullet.cpp
    Bullet/Bullet.hpp
    Bullet/MachineGunBullet.cpp
    Bullet/MachineGunBullet.hpp
    Bullet/LaserBullet.cpp
    Bullet/LaserBullet.hpp
    Bullet/EagleBullet.cpp 
    Bullet/EagleBullet.hpp 
    Bullet/StunBullet.cpp 
    Bullet/StunBullet.hpp 
    Bullet/TeslaBullet.cpp 
    Bullet/TeslaBullet.hpp 

    # TODO HACKATHON-3 (3/3): Add the new enemy source files here
    Enemy/Enemy.cpp
    Enemy/Enemy.hpp
    Enemy/SoldierEnemy.cpp
    Enemy/SoldierEnemy.hpp
    Enemy/TankEnemy.cpp
    Enemy/TankEnemy.hpp
    Enemy/PlaneEnemy.cpp 
    Enemy/PlaneEnemy.hpp
    Enemy/HealerEnemy.cpp 
    Enemy/HealerEnemy.hpp
    Enemy/RedTankEnemy.cpp
    Enemy/RedTankEnemy.hpp

    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp
    
    # TODO HACKATHON-2 (3/3): Add the new scene source files here
    Scene/LoseScene.cpp
    Scene/LoseScene.hpp
    Scene/PlayScene.cpp
    Scene/PlayScene.hpp
    Scene/StageSelectScene.cpp
    Scene/StageSelectScene.hpp
    Scene/StartScene.cpp
    Scene/StartScene.hpp
    Scene/WinScene.cpp
    Scene/WinScene.hpp
    Scene/SettingsScene.cpp 
    Scene/SettingsScene.hpp
    Scene/ScoreboardScene.cpp
    Scene/ScoreboardScene.hpp

    Turret/LaserTurret.cpp
    Turret/LaserTurret.hpp
    Turret/MachineGunTurret.cpp
    Turret/MachineGunTurret.hpp
    Turret/EagleTurret.cpp 
    Turret/EagleTurret.hpp
    Turret/StunTurret.cpp 
    Turret/StunTurret.hpp 
    Turret/TeslaTurret.cpp 
    Turret/TeslaTurret.hpp
    Turret/Turret.cpp
    Turret/Turret.hpp
    Turret/TurretButton.cpp
    Turret/TurretButton.hpp
    Turret/Shovel.cpp 
    Turret/Shovel.hpp

    UI/Animation/DirtyEffect.cpp
    UI/Animation/DirtyEffect.hpp
    UI/Animation/ExplosionEffect.cpp
    UI/Animation/ExplosionEffect.hpp
    UI/Animation/AreaEffect.cpp 
    UI/Animation/AreaEffect.hpp
    UI/Animation/Plane.cpp
    UI/Animation/Plane.hpp

    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp
    
    main.cpp
)

IF (WIN32)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/include)
    target_link_directories(2025_I2P2_TowerDefense PUBLIC C:/allegro/lib)
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC lib)
    target_include_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_TowerDefense ${ALLEGRO5_LIBRARIES})
    target_link_directories(2025_I2P2_TowerDefense PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()