//
// Fichier qui permet de faire le transfert de fichier entre la cible et le client
//


#ifndef CLIENT___PIRATE_FICHIERDETRANSFER_H
#define CLIENT___PIRATE_FICHIERDETRANSFER_H

#include <fstream>
struct FichierDeTransfer
{
    static const int buffersize = 8192;
    std::string fileName;
    int bytesWritten;
    std::streampos fileOffset;
    std::streampos fileSize;
    int remainingBytes;
    std::ifstream infileStream;
    std::ofstream outfileStream;
    char buffer[buffersize];
};

#endif //CLIENT___PIRATE_FICHIERDETRANSFER_H
