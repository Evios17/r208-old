#include "PGN.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace pgnp;

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define RESET "\033[0m"

int main () {

    std::cout << CYAN "Début de la fonction main." RESET << std::endl;

    // Insérer le fichier PGN dans le parser
    PGN pgn;
    pgn.FromFile("example2.pgn");

    std::cout << YELLOW "Fichier trouvé, tentative de conversion." RESET << std::endl;

    // Créer le fichier de sortie et création du buffer
    std::ofstream outfile("final.tex");
    std::stringstream buffer;

    // Création de l'entête du document
    buffer
    << "\\documentclass{article}\n"
    << "\\usepackage{multicol}\n"
    << "\\usepackage{array}\n"
    << "\\usepackage{makeidx}\n"
    << "\\usepackage[skaknew]{chessfss}\n"
    << "\\usepackage{texmate}\n"
    << "\\usepackage{xskak}\n"
    << "\\usepackage[top=1.5cm, bottom=2cm, left=1.5cm, right=1cm,headheight=15pt]{geometry}\n"
    << "\\usepackage{adjmulticol}\n"
    << "\\usepackage{ragged2e}\n\n\n"
    << "\\begin{document}\n\n";

    // Variable pour conmpter le nombre de de games
    int count=1;

    while (true) {

        try {

            pgn.ParseNextGame();

            std::cout << "Game n°" << count << ":" <<std::endl;
            count++;

        } catch (const NoGameFound& e) {

            std::cout << RED "Pas de nouvelle game trouvé, fin du fichier atteint." RESET << std::endl;
            break;

        }

        std::cout << PURPLE "J'ai passé le try/catch !" RESET << std::endl;

        HalfMove *m = new HalfMove();
        pgn.GetMoves(m);

        // Trouver les commentaires :

        // Entête et corps du jeu actuel
        buffer
        << "\\chessevent{" << pgn.GetTagValue("Event") << "}\n"

        << "\\chessopening{" << pgn.GetTagValue("Site") << "}\n\n"

        << "Date : " << pgn.GetTagValue("Date") << "\n\n"
        << "Round : " << pgn.GetTagValue("Round") << "\n\n"
        << "Result : " << pgn.GetResult() << "\n\n"

        << "\\whitename{" << pgn.GetTagValue("White") << "}\n\n"
        << "\\blackname{" << pgn.GetTagValue("Black") << "}\n\n"
        << "\\ECO{" << pgn.GetTagValue("ECO") << "}\n\n"
        << "\\whiteelo{?}\n\n"
        << "\\blackelo{?}\n\n"
        << "Plycount : " << pgn.GetTagValue("PlyCount")  << "\n\n"

        << "\\makegametitle\n"
        << "\\begin{multicols}{2}\n"
        << "\\noindent\n"
        << "\\newchessgame[id=main]\n"
        << "\\xskakset{style=styleC}\n"

        << "\\mainline{";

        int hmperMain = 4;                                                      // Nombre désiré de count avant affichage du plateau visuel
        int movesnb = hmperMain*2;                                              // Conversion nombre de count en nombre de half-moves (x2)
        bool secondhalf = false;                                                // Compte du half-move actuel dans le count, contient deux position : "0" et "1" pour représenter le premier et le deuxième half-move du count
        int hmcount = 0;                                                        // Compte du half-move actuel dans la mainline
        bool firstmove = true, mainline_end = false, commenthere = false;       // Valeurs booleans pour les conditions

        std::cout << YELLOW "Nombres de half-moves : " << m->GetLength() << RESET << std::endl;

        for (int i=0 ; i < m->GetLength() ; i++) {

            // Si la mainline est complète
            if (mainline_end == true) {

                hmcount = 1;
                mainline_end = false;

            }

            // Insertion de fin de mainline si la mainline est complète
            if (hmcount == movesnb) {

                buffer 
                << "}\n"
                << "\\scalebox{0.90}{\\chessboard}\\\\\n"
                << "\\mainline{";

            }

            // Conditions d'affichage du count
            // Si c'est le tout premier half-move de la game, insérer le count
            if (firstmove == true) {

                buffer << m->GetHalfMoveAt(i)->count << ". ";
                firstmove = false;

            // Si on est sur le deuxième half-move du count actuel, insérer le count et réinitialiser le comptage du half-move par count
            } else if (secondhalf == true) {

                buffer << m->GetHalfMoveAt(i)->count << ". ";
                secondhalf = false;

            // Incrémenter le comptage de count
            } else {

                secondhalf = true;

            }

            // Si le nombre de half-move est pas équivaut au nombre maximum que peut contenir une mainline, incrémenter le compte de half-move
            if (hmcount != movesnb) {

                hmcount++;

            // Si le nombre est équivaut, initier la variable d'insertion de fin de mainline
            } else {

                mainline_end = true;

            }

            // Insertion du half-move dans la mainline
            buffer << m->GetHalfMoveAt(i)->move << " ";

            // Insertion de commentaire si présent
            if (!m->GetHalfMoveAt(i)->comment.empty()) {

                buffer
                << "}\n"
                << "\\xskakcomment{\\small\\texttt\\justifying{\\textcolor{darkgray}{" << m->GetHalfMoveAt(i)->comment << "}}}\n"
                << "\\mainline{";

                // Si ce n'est pas le tout dernier half-move et que le half-move actuel n'est pas le deuxième half-move du count, insérer le count pour reprendre le count incomplet
                if (i != (m->GetLength()-1) && secondhalf != true) {

                    buffer << m->GetHalfMoveAt(i)->count << "... ";

                }

            }

        }

        buffer << "}\n";
        buffer << "\\end{multicols}\n";

        buffer << "\\newpage\n\n";

        std::cout << std::endl;

    }
    
    buffer << "\\end{document}";

    outfile << buffer.str();

    std::cout << CYAN "Fin du programme." RESET << std::endl;

    return EXIT_SUCCESS;
}
