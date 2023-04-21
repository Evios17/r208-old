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

int main (int argc, char *argv[]) {

    #ifdef _WIN32
        printf(ROUGE"Ce programme n'est pas compatible avec les systèmes Windows\n");
        return EXIT_FAILURE;
    #endif

    int opt;

    std::string file_input;
    std::string file_output;
    bool output = false;

    // Nombre désiré de count par défaut avant affichage du plateau visuel
    int hmperMain = 2;

    bool debug = false;

    while ((opt = getopt (argc, argv, "i:o:n:sdhv")) != -1) {
        switch (opt) {
            case 'i' :                                                                                            // -s : active le mode "serveur"
                file_input = optarg;

                std::cout << "Nom du fichier en entrée : " << file_input << "\n" << std::endl;

                break;
            case 'o' :                                                                                            // -s : active le mode "serveur"
                output = true;

                file_output = optarg;

                std::cout << "Nom du fichier en sortie : " << file_output << "\n" << std::endl;

                break;
            case 'n' :                                                                                            // -s : active le mode "serveur"
                hmperMain = atoi(optarg);

                break;
            case 's' :                                                                                            // -s : active le mode "serveur"
                puts("in -o");
                return(EXIT_SUCCESS);

                break;
            case 'd' :                                                                                            // -s : active le mode "serveur"
                debug = true;
                return(EXIT_SUCCESS);

                break;
            case 'h' :                                                                                            // -h : affiche l'aide
                std::cout << "Info :" << std::endl;
                std::cout << "Licence\t -- GPL" << std::endl;
                std::cout << "Author\t -- Product by Antoine DORO, Lukas BOYER and Cylian HOUTMANN" << std::endl;
                std::cout << "Description\t -- \n" << std::endl;
                
                std::cout << "Options :" << std::endl;
                std::cout << "-i\t -- Path of the input PGN file" << std::endl;
                std::cout << "-o\t -- Name of the output TEX file" << std::endl;
                std::cout << "-n\t -- Number of moves before showing the chessboard" << std::endl;
                std::cout << "-s\t -- Turn on stat mode" << std::endl;
                std::cout << "-d\t -- Turn on debug mode" << std::endl;
                std::cout << "-h\t -- Show help" << std::endl;
                std::cout << "-v\t -- Show version" << std::endl;
                exit(EXIT_SUCCESS);

                break;
            case 'v':                                                                                            // -v : affiche la version
                std::cout << "BlackLatex version 1.0" << std::endl;
                exit(EXIT_SUCCESS);

                break;
            default :
                std::cout << RED "[ERROR] Wrong syntax, you can use -h to see options." RESET << std::endl;

                exit(EXIT_FAILURE);
        }
        
    }

    std::cout << CYAN "Début de la fonction main." RESET << std::endl;

    if(file_input == ""){
        std::cout << YELLOW "Fichier non trouvé, veuillez indiquer un fichier à convertir." RESET << std::endl;
        std::cout << CYAN "Fin du programme." RESET << std::endl;
        
        return EXIT_FAILURE;
    } else {
        std::cout << YELLOW "Fichier trouvé, tentative de conversion." RESET << std::endl;
    }
    
    // Insérer le chemin du fichier PGN dans le parser
    PGN pgn;
    pgn.FromFile(file_input);

    // Créer le fichier de sortie et création du buffer
    
    // Vérifie si l'on a choisis de donner un nom au fichier ".tex". Si tel n'est pas le cas, on nomme 
    std::string outname;

    if(output == true){
        outname = "output/" + file_output + ".tex";
    } else {

        file_output = file_input.substr((file_input.find_last_of("/") + 1 ));

        file_output = file_output.substr(0, file_output.length() - 4);
        
        outname = "output/" + file_output + ".tex";
    }
    
    std::ofstream outfile(outname);
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

    // Variable pour compter le nombre de games
    int count=1;

    while (true) {

        try {

            pgn.ParseNextGame();

            std::cout << "Game n°" << count << ":" <<std::endl;
            count++;

        } catch (const NoGameFound& e) {

            std::cout << RED "Pas de nouvelle game trouvée, fin du fichier atteint." RESET << std::endl;
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

        int movesnbr = hmperMain*2;                                             // Conversion nombre de count en nombre de half-moves (x2)
        bool secondhalf = false;                                                // Compte du half-move actuel dans le count, contient deux position : "0" et "1" pour représenter le premier et le deuxième half-move du count
        int hmcount = 0;                                                        // Compte du half-move actuel dans la mainline
        bool firstmove = true, mainline_end = false, commenthere = false;       // Valeurs booleans pour les conditions

        std::cout << YELLOW "Nombres de half-moves : " << m->GetLength() << RESET << std::endl;

        for (int i=0 ; i < m->GetLength() ; i++) {

            // Initiation de variable 
            int varORcom = false;

            // Si la mainline est complète
            if (mainline_end == true) {

                hmcount = 1;
                mainline_end = false;

            }

            // Insertion de fin de mainline si la mainline est complète
            if (hmcount == movesnbr) {

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
            if (hmcount != movesnbr) {

                hmcount++;

            // Si le nombre est équivaut, initier la variable d'insertion de fin de mainline
            } else {

                mainline_end = true;

            }

            // Insertion du half-move dans la mainline
            buffer << m->GetHalfMoveAt(i)->move << " ";

            // Insertion de commentaire si présent
            if (!m->GetHalfMoveAt(i)->comment.empty()) {

                varORcom = true;

                buffer
                << "}\n"
                << "\\xskakcomment{\\small\\texttt\\justifying{\\textcolor{darkgray}{" << m->GetHalfMoveAt(i)->comment << "}}}\n";

            }

            // Insertion de variations
            // Si il y a une variation dans le move actuel, initier l'écriture des varations
            if (m->GetHalfMoveAt(i)->variations.size() != 0) {

                // Comptage de variante dans le half-move
                int varcount = 1;

                // Si un commentaire n'as pas été écrit pendant ce half-move, prendre l'initiation de fermer le mainline actuel
                if (varORcom != true) {

                    // Fermer la mainline actuelle
                    buffer << "}\n";            

                // Si présence d'un commentaire, uniquement retourner à la ligne
                } else {

                    buffer << "\n";

                } 

                // Initier la variable de reouverture de mainline
                varORcom = true;                

                // Permet de savoir quand on passe à la deuxième variante si le half-move contient plusieurs varientes
                bool multvar = false;           

                // Boucler à chaque variation
                for (int j=0; j < m->GetHalfMoveAt(i)->variations.size(); j++) {

                    // Si le nombre de variation est supérieur à 1, afficher les numéros de variantes
                    if (m->GetHalfMoveAt(i)->variations.size() > 1) {

                        // Quand arrivé à la deuxième variation, passer à la ligne
                        if ( multvar == true ) {

                            buffer << "\n";

                        }

                        buffer << "\\xskakcomment{\\small\\texttt\\justifying{\\textcolor{gray}{Variant n°" << varcount << ": ";
                        varcount++;

                        multvar = true;

                    // Si il n'y a qu'une seule variation dans le half-move
                    } else {

                            buffer << "\\xskakcomment{\\small\\texttt\\justifying{\\textcolor{gray}{Variant : ";

                    }

                    buffer << "\\variation{" << m->GetHalfMoveAt(i)->variations[j]->count;
                    
                    // Si il s'agit du move blanc
                    if (m->GetHalfMoveAt(i)->variations[j]->isBlack != 1) {

                        buffer << ". ";

                    // S'il s'agit du move noir
                    } else {

                        buffer << "... ";

                    }
                    
                    buffer << m->GetHalfMoveAt(i)->variations[j]->move << "} ";

                    // Si il y a un commentaire dans la variation, l'insérer
                    if (!m->GetHalfMoveAt(i)->variations[j]->comment.empty()) {

                        buffer << m->GetHalfMoveAt(i)->variations[j]->comment << " ";

                    }

                    buffer << "}}}\n";

                }

            }

            // Réouverture d'une mainline si la précédente a été intérompue par une variant ou un commentaire
            if (varORcom == true) {

                buffer
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
    
    // Fin du document
    buffer << "\\end{document}";

    outfile << buffer.str();

    std::cout << CYAN "Fin du programme." RESET << std::endl;

    return EXIT_SUCCESS;
}