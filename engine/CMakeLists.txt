target_sources(engine PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/core/src/application.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/device.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/pipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/swap_chain.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/window.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/vertex_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/renderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/renderer_system.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/camera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/index_buffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/core/src/game_object.cpp)