#include <iostream>
#include <fstream>

bool isLexLess(std::string a, std::string b) { // a is lexically less than b
    for (unsigned int i = 0; i < std::min(a.size(), b.size()); i++) {
        if (a[i] < b[i]) {
            return true;
        }
        if (a[i] > b[i]) {
            return false;
        }
    }
    return a.size() < b.size();
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 3) {
        std::cout << "Wrong amount of arguments. Provide filenames as arguments. See example below:\n";
        std::cout << "comm flag File1 File2" << std::endl;
        return 0;
    }
    std::ifstream inputFile1(argv[argc - 2]);
    if (!inputFile1.good()) {
        std::cout << "Can't open source file " << argv[2] << ". Verify that the file exists." << std::endl;
        return 0;
    }
    std::ifstream inputFile2(argv[argc - 1]);
    if (!inputFile2.good()) {
        std::cout << "Can't open source file " << argv[3] << ". Verify that the file exists." << std::endl;
        return 0;
    }
    bool flag1 = true; // flag of first column
    bool flag2 = true; // flag of second column
    bool flag3 = true; // flag of third column
    if (argc == 4) {
        std::string opt = argv[1];
        if (opt[0] != '-') {
            return 0;
        }
        for (int i = 1; i < static_cast<int>(opt.size()); i++) {
            switch (opt[i]) {
                case '1' :
                    flag1 = false;
                    break;
                case '2' :
                    flag2 = false;
                    break;
                case '3' :
                    flag3 = false;
                    break;
                default :
                    return 0;
            }
        }
    }
    std::string one;
    std::string two;
    if (inputFile1.peek() != EOF) {
        getline(inputFile1, one);
        bool check = false; // (check = true) => inputFile1 is checked
        while (inputFile2.peek() != EOF && !check) {
            getline(inputFile2, two);
            while (isLexLess(one, two) && !check) {
                if (flag1) {
                    std::cout << one << std::endl; // element of the first column, unique for file1
                }
                if (inputFile1.peek() == EOF) {
                    check = true;
                } else {
                    getline(inputFile1, one);
                }
            }
            if (!check) {
                if (one == two) {
                    if (flag3) {
                        if (flag1) {
                            std::cout << "\t";
                        }
                        if (flag2) {
                            std::cout << "\t";
                        }
                        std::cout << one << std::endl; // element of the third column, appears in both files
                    }
                    if (inputFile1.peek() == EOF) {
                        check = true;
                    } else {
                        getline(inputFile1, one);
                    }
                } else {
                    if (flag2) {
                        if (flag1) {
                            std::cout << "\t";
                        }
                        std::cout << two << std::endl; // element of the second column, unique for file2
                    }
                }
            }
        }
        if (!check && flag1) { // file2 is checked, file1 not
            std::cout << one << std::endl;
            while (inputFile1.peek() != EOF) {
                getline(inputFile1, one);
                std::cout << one << std::endl; // element of the first column, unique for file1
            }
        }
        if (check && (one != two) && flag2) { // file1 is checked, "two" hasn't yet been written
            if (flag1) {
                std::cout << "\t";
            }
            std::cout << two << std::endl; // element of the second column, unique for file2
        }
    }
    if (flag2) { // file1 is checked, file2 not
        while (inputFile2.peek() != EOF) {
            getline(inputFile2, two);
            if (flag1) {
                std::cout << "\t";
            }
            std::cout << two << std::endl; // element of the second column, unique for file2
        }
    }
    return 0;
}
