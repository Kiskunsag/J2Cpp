#include <iostream>
#include <QtCore/QCoreApplication>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

/*int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout << a.applicationVersion().toStdString() << endl;
    return a.exec();
}*/

void printHelp()
{
    cout << "Usage:" << endl;
    cout << "-h         Print this help message." << endl;
    cout << "-c [files] Convert selected files from Java to C++." << endl;
    cout << "-r         Recursivly converts every file that is included from a file specified in [files]." << endl;
}

int main(int argc, char *argv[])
{
    cout << "JavaToCpp-Converter" << endl;
    cout << "Max Beikirch 2012" << endl;

    string fileName;

    char c;
    while ((c = getopt (argc, argv, "c:hr")) != -1)
        switch (c)
        {
        // convert
        case 'c' :
        {
            fileName = optarg;
            break;
        }
        case 'h':
        {
            printHelp();
            break;
        }
        default: ;
            //abort();
        }

    return 0;
}


