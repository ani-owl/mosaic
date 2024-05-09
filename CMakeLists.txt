set(CMAKE_VERBOSE_MAKEFILE ON)
link_directories(${CMAKE_SOURCE_DIR})
cmake_minimum_required(VERSION 3.2)
project(game_text C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g -Wall")
set(CMAKE_C_FLAGS_DEBUG "-g --coverage")
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -O3 -Wall")
include(CTest)
enable_testing()


include(sdl2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

## compilation rules
include_directories(${SDL2_ALL_INC})
add_executable(game_text game_text.c )
add_executable(game_test_nsemenov game_test_nsemenov.c)
add_executable(game_test_ikozak game_test_ikozak.c)
add_executable(game_test_ahadjamar game_test_ahadjamar.c )
add_executable(game_test_ext game_test_ext.c)
add_executable(game_solve game_solve.c )

add_executable(game_sdl model.c  game_sdl.c )
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m game)

add_library(game game.c game_aux.c game_ext.c queue.c game_private.c game_tools.c game_random.c )


target_link_libraries(game_text game  m)
target_link_libraries(game_test_ahadjamar game m)
target_link_libraries(game_test_nsemenov game  m)
target_link_libraries(game_test_ikozak game  m)
target_link_libraries(game_test_ext game m)
target_link_libraries(game_solve game m)

add_test(test_nsemenov_dummy ./game_test_nsemenov dummy)
add_test(test_nsemenov_game_default_solution ./game_test_nsemenov game_default_solution)
add_test(test_nsemenov_game_restart ./game_test_nsemenov game_restart)
add_test(test_nsemenov_game_nb_neighbors ./game_test_nsemenov game_nb_neighbors)
add_test(test_nsemenov_game_get_color ./game_test_nsemenov game_get_color)
add_test(test_nsemenov_game_set_constraint ./game_test_nsemenov game_set_constraint)
add_test(test_nsemenov_game_copy ./game_test_nsemenov game_copy)


add_test(test_ahadjamar_dummy ./game_test_ahadjamar dummy)
add_test(test_ahadjamar_game_equal ./game_test_ahadjamar game_equal)
add_test(test_ahadjamar_game_set_color ./game_test_ahadjamar game_set_color)
add_test(test_ahadjamar_game_get_next_square ./game_test_ahadjamar game_get_next_square_wrapping)
add_test(test_ahadjamar_game_get_next_square ./game_test_ahadjamar game_get_next_square_not_wrapping)
add_test(test_ahadjamar_game_new ./game_test_ahadjamar game_new)
add_test(test_ahadjamar_game_play_move ./game_test_ahadjamar game_play_move)
add_test(test_ahadjamar_game_print ./game_test_ahadjamar game_print)
add_test(test_ahadjamar_game_load ./game_test_ahadjamar game_load)
add_test(test_ahadjamar_game_save ./game_test_ahadjamar game_save)
add_test(test_ahadjamar_game_solve ./game_test_ahadjamar game_solve)


add_test(test_ikozak_dummy ./game_test_ikozak dummy)
add_test(test_ikozak_game_delete ./game_test_ikozak game_delete)
add_test(test_ikozak_game_default ./game_test_ikozak game_default)
add_test(test_ikozak_game_won ./game_test_ikozak game_won)
add_test(test_ikozak_game_new_empty ./game_test_ikozak game_new_empty)
add_test(test_ikozak_game_get_constraint ./game_test_ikozak game_get_constraint)
add_test(test_ikozak_game_get_status ./game_test_ikozak game_get_status) 
add_test(test_ikozak_game_nb_solutions ./game_test_ikozak game_nb_solutions) 


add_test(test_ext_game_new_ext ./game_test_ext game_new_ext)
add_test(test_ext_game_new_empty_ext ./game_test_ext game_new_empty_ext) 
add_test(test_ext_game_nb_rows ./game_test_ext game_nb_rows) 
add_test(test_ext_game_nb_cols ./game_test_ext game_nb_cols) 
add_test(test_ext_game_is_wrapping ./game_test_ext game_is_wrapping) 
add_test(test_ext_game_get_neighbourhood ./game_test_ext game_get_neighbourhood) 
add_test(test_ext_game_undo ./game_test_ext game_undo) 
add_test(test_ext_game_redo ./game_test_ext game_redo) 

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/default.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/solution.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/save.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# folder copy
