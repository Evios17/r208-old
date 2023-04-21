#add_compile_options(-Wall -Wextra -pedantic -Werror)

# Nous voulons un cmake "récent" pour utiliser les dernières fonctionnalités

cmake_minimum_required(VERSION 3.18.4)

# Notre projet est étiqueté tp1

project(LatexConvertor)

# Inclure pgnp
add_subdirectory(pgnp)

# Création exécutable

add_executable(main main.cpp)
# Liens de la cible avec les bibliothèques externes
target_include_directories(main PRIVATE pgnp/src pgnp/libs/chess-move-interface/src)
target_link_libraries(main PRIVATE pgnp)