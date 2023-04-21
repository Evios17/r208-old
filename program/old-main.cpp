/*#include "PGN.hpp"
#include <iostream>
using namespace pgnp;

int main(){
    PGN pgn;
    pgn.FromFile("example.pgn");
    pgn.ParseNextGame();

    std::cout << "Event is: " << pgn.GetTagValue("Event") << std::endl;

    HalfMove *m = new HalfMove();
    pgn.GetMoves(m);
    std::cout << "First move is: " << m->move << std::endl;
    return(0);
}*/



#include "PGN.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace pgnp;

#define GREEN "\x1b[38;2;34;230;126m"
#define ORANGE "\x1b[38;2;255;100;0m"
#define RED "\x1b[38;2;255;0;0m"
#define GREY "\x1b[38;2;204;204;204m"
#define WHITE "\x1b[38;2;255;255;255m"
#define RESET "\033[0m"


void output(int value);
void generator(char file_name[]);


int main(int argc, char **argv){

    int opt, arg;

    if ( argc > 2 ) {                                                                                           // Teste les options reçu dans le main
        puts(RED "Only one flag can be used at a time" RESET);
        puts("Usage :");
        puts("-f\t -- Use a file as argument");
        puts("-v\t -- Show version");
        exit(1);
    }
    else {
        while ((opt = getopt (argc, argv, ":fh")) != -1) {
            switch (opt) {
                case 'f' :                                                                                            // -c : active le mode "client"
                    
                    arg = true;
                    break;
                case 'h' :                                                                                            // -h : affiche l'aide
                    puts("Usage :");
                    puts("-f\t -- Use a file as argument");
                    puts("-v\t -- Show version");
                    exit(0);
                    break;
                case 'v':                                                                                            // -v : affiche la version
                    puts("PGN to LaTeX converter v1, made by Antoine, Lukas and Cylian #BUTRT");
                    exit(0);
                    break;
                default :                                                                                            // Affiche l'aide quand l'utilisateur entre un argument non-existant
                    printf(RED "Unknown flag: \"%s\"" RESET,opt);
                    puts("Usage :");
                    puts("-f\t -- Use a file as argument");
                    puts("-v\t -- Show version");
                    exit(0);
            }
        }
    }

    // Generator
    char file_name[] = "example2";
    generator(file_name);


    return(0);
}

void output(int value){
    switch(value){
        case 1:
            puts( GREEN "[Run] Loading file.. " WHITE "exemple.pgn" RESET);
            break;
        case 2:
            puts( GREEN "[Run] Converting file.. " RESET);
            break;
        case 3:
            puts( GREEN "[Run] The file " WHITE "exemple.tex " GREEN "as been created successfully !" RESET);
            break;
        default :
            puts(RED "[Error] Output undifine" RESET);
    }
}

void generator(char file_name[]){

    bool debug = false;
    bool stat = true;

    // Send a loading message
    output(1);

    // Load the file and parse him
    int array_size = strlen(file_name);
    char file_name_tmp[array_size];
    strcpy(file_name_tmp, file_name);

    PGN pgn;
    pgn.FromFile(strcat(file_name_tmp, ".pgn"));
    
    // Send a convert message
    output(2);
    
    // Create the latex file or open a exiting file
    strcpy(file_name_tmp, file_name);
    std::ofstream outfile(strcat(file_name_tmp, ".tex"));

    // Create tampon
    std::stringstream buffer;

    
    // [Latex] Add packages
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
    << "\\usepackage{ragged2e}\n\n\n";
        
    // [Latex] Create document
    buffer << "\\begin{document}\n\n";
    
    // [Latex] Add game raound
    bool toggle = false;
    int round = 1;

    int stat1 = 0;
    int stat2 = 0;
    int stat3 = 0;
    int stat4 = 0;

    while (true){
        
        try {
            pgn.ParseNextGame();
            
            if (debug == true){
                std::cout << ORANGE "[Debug] Round : " << round << RESET << std::endl;
            }
            
            round++;
        } catch (const NoGameFound& e) {
            if (debug == true){
                std::cout << ORANGE "[Debug] No more games found" RESET << std::endl;
            }
            
            break;
        }


        // [Latex] Add header
        
        
        buffer
        << "\\chessevent{" << pgn.GetTagValue("Event") << "}\n"
        << "\\chessopening{" << pgn.GetTagValue("Site") << "}\n\n"

        << "Date : " << pgn.GetTagValue("Date") << "\n"
        << "EventDate : " << pgn.GetTagValue("EventDate") << "\n"
        << "Round : " << pgn.GetTagValue("Round") << "\n"
        << "Result : " << pgn.GetTagValue("Result") << "\n"
        << "\\whitename{" << pgn.GetTagValue("White") << "}" << "\n"
        << "\\blackname{" << pgn.GetTagValue("Black") << "}" << "\n";
        //<< "\\ECO{" << pgn.GetTagValue("ECO") << "}" << "\n"
        //<< "\\whiteelo{" << pgn.GetTagValue("WhiteElo") << "}" << "\n"
        //<< "\\blackelo{" << pgn.GetTagValue("BlackElo") << "}" << "\n"
        //<< "PlyCount : " << pgn.GetTagValue("PlyCount") << "\n" << "\n\n";

        // [Latex] Add body
        buffer
        << "\\makegametitle\n"
        << "\\begin{multicols}{2}\n"
        << "\\noindent\n"
        << "\\newchessgame[id=main]\n"
        << "\\xskakset{style=styleC}\n";

        HalfMove *m = new HalfMove();
        pgn.GetMoves(m);

        int range = 5;
        int length = m->GetLength();
        std::string comt = "";

        stat1 = length + stat1;

        int bcl = 0;

        // Calcul
        length++;

        int lengthMod = length % 2;
        length = (length - lengthMod) / 2;

        int remainder = length % range;
        int quotient = length / range;

        if(remainder > 0){
            quotient++;
        }

        if (debug == true){
            std::cout << ORANGE "[Debug] HalfMove : " << length << RESET << std::endl;
            std::cout << ORANGE "[Debug] Reste : " << remainder << RESET << std::endl;
            std::cout << ORANGE "[Debug] Quotien : " << quotient << RESET << std::endl;
        }
        
        for(int loop1 = 0; loop1 < quotient; loop1++){
            
            if (debug == true){
                std::cout << ORANGE << loop1 << RESET << ", " << std::endl;
            }

            buffer << "\\mainline{";

            if (remainder > 0){
                if (loop1 == (quotient--)) {
                    for (int yzx = 0; yzx < remainder; yzx++) {
                        buffer << m->GetHalfMoveAt(bcl)->count << ". ";
                        
                        if (debug == true){
                            std::cout << m->GetHalfMoveAt(bcl)->count << ", ";
                        }

                        if (bcl == ((length * 2) -2)) {
                            buffer << m->GetHalfMoveAt(bcl)->move  << " ";
                            
                            if (m->GetHalfMoveAt(bcl)->comment.empty()) {

                            } else {
                                comt = comt + m->GetHalfMoveAt(bcl)->comment + "; ";
                            }

                            if (debug == true){
                                std::cout << m->GetHalfMoveAt(bcl)->move << std::endl;
                                std::cout << ORANGE "  [Debug] Je suis passé par là" RESET << std::endl;
                            }
                            

                            //bcl++;
                        } else {
                            for (int zxy = 0; zxy < 2; zxy++) {
                                buffer << m->GetHalfMoveAt(bcl)->move  << " ";
                                
                                if (m->GetHalfMoveAt(bcl)->comment.empty()) {

                                } else {
                                    comt = comt + m->GetHalfMoveAt(bcl)->comment + "; ";
                                }

                                if (debug == true){
                                    std::cout << m->GetHalfMoveAt(bcl)->move << std::endl;
                                }

                                bcl++;
                            }
                        }
                    }
                } else {
                    for (int yzx = 0; yzx < range; yzx++) {
                        buffer << m->GetHalfMoveAt(bcl)->count << ". ";
                        
                        if (debug == true){
                            std::cout << m->GetHalfMoveAt(bcl)->count << ", ";
                        }

                        for (int zxy = 0; zxy < 2; zxy++) {
                            buffer << m->GetHalfMoveAt(bcl)->move  << " ";
                            
                            if (m->GetHalfMoveAt(bcl)->comment.empty()) {

                            } else {
                                comt = comt + m->GetHalfMoveAt(bcl)->comment + "; ";
                            }

                            if (debug == true){
                                std::cout << m->GetHalfMoveAt(bcl)->move << std::endl;
                            }

                            bcl++;
                        }
                    }
                }
                
            } else {
                for (int yzx = 0; yzx < range; yzx++) {
                    buffer << m->GetHalfMoveAt(bcl)->count << ". ";
                    
                    if (debug == true){
                        std::cout << m->GetHalfMoveAt(bcl)->count << ", ";
                    }

                    if (bcl == ((length * 2) -2)) {
                        buffer << m->GetHalfMoveAt(bcl)->move  << " ";
                        
                        if (m->GetHalfMoveAt(bcl)->comment.empty()) {

                        } else {
                            comt = comt + m->GetHalfMoveAt(bcl)->comment + "; ";
                        }

                        if (debug == true){
                            std::cout << m->GetHalfMoveAt(bcl)->move << std::endl;
                            std::cout << ORANGE "  [Debug] Je suis passé par là" RESET << std::endl;
                        }

                        //bcl++;
                    } else {
                        for (int zxy = 0; zxy < 2; zxy++) {
                            buffer << m->GetHalfMoveAt(bcl)->move  << " ";
                            
                            if (m->GetHalfMoveAt(bcl)->comment.empty()) {

                            } else {
                                comt = comt + m->GetHalfMoveAt(bcl)->comment + "; ";
                            }

                            if (debug == true){
                                std::cout << m->GetHalfMoveAt(bcl)->move << std::endl;
                            }

                            bcl++;
                        }
                    }
                }
            }
            
            
            buffer << "}\n";

            if (comt.empty()) {
                buffer << "\\scalebox{0.90}{\\chessboard}\n";
            } else {
                buffer << "\\xskakcomment{\\small\\texttt\\justifying{\\textcolor{darkgray}{~ " << comt << " }}}\n\n" << "\\scalebox{0.90}{\\chessboard}\n";
            }
        }

        buffer
        << "\nScore : " << pgn.GetResult() << "\n\n"
        << "\\end{multicols}\n\n"
        << "\\newpage\n\n";
    }

    if (stat == true){
        std::cout << GREY "[Stat] Total round : " << round << RESET << std::endl;
        std::cout << GREY "[Stat] Total HalfMove : " << stat1 << RESET << std::endl;
    }
    
    // Fermeture du document
    buffer << "\\end{document}";

    // Copy to the file
    outfile << buffer.str() << std::endl;

    // Closing file
    outfile.close();

    // Send a success message
    output(3);
}