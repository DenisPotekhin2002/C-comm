#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

const size_t colNum = 3;

std::vector<bool> assign_f(int n, const std::string &opt) { //parse flags for columns
    std::vector<bool> flags;
    for (size_t i = 0; i < colNum; i++) {
        flags.push_back(true);
    }
    if (n == 4) {
        assert(opt[0] == '-');
        for (size_t i = 1; i < opt.size(); i++) {
            flags[opt[i] - '1'] = false;
        }
    }
    return flags;
}

void print_1(std::string &one, std::ostream &out, const std::vector<bool> &flags) { //first column
    if (flags[0]) {
        out << one << std::endl;
    }
}

void print_2(std::string &two, std::ostream &out, const std::vector<bool> &flags) { //second column
    if (flags[1]) {
        if (flags[0]) {
            out << "\t";
        }
        out << two << std::endl;
    }
}

void print_3(std::string &three, std::ostream &out, const std::vector<bool> &flags) { //third column
    if (flags[2]) {
        if (flags[0]) {
            out << "\t";
        }
        if (flags[1]) {
            out << "\t";
        }
        out << three << std::endl;
    }
}

void check_both(std::ifstream &inputFile1, std::ifstream &inputFile2, std::string &one, std::string &two,
                const std::vector<bool> &flags, std::ostream &out) {
    while (true) {
        while (two < one) { // read lines from 2, while less than one
            print_2(two, out, flags);
            if (inputFile2.peek() != EOF) { // next line of file2, if exists
                getline(inputFile2, two);
            } else {
                print_1(one, out, flags); // one is read, but not yet written
                return;
            }
        }
        while (one < two) { // read lines from 1, while less than two
            print_1(one, out, flags);
            if (inputFile1.peek() != EOF) { // next line of file1, if exists
                getline(inputFile1, one);
            } else {
                print_2(two, out, flags); // two is read, but not yet written
                return;
            }
        }
        while (one == two) {
            print_3(one, out, flags);
            if (inputFile1.peek() != EOF && inputFile2.peek() != EOF) { // next line of each file, if exist
                getline(inputFile1, one);
                getline(inputFile2, two);
            } else {
                return;
            }
        }
    }
    //check_both(inputFile1, inputFile2, one, two, flags, out);
    //if you don't like loop "while(true)", this can be used instead
}

void compare(std::ifstream &inputFile1, std::ifstream &inputFile2, const std::vector<bool> &flags, std::ostream &out) {
    std::string one;
    std::string two;
    if (inputFile1.peek() != EOF && inputFile2.peek() != EOF) {
        getline(inputFile1, one);
        getline(inputFile2, two);
        check_both(inputFile1, inputFile2, one, two, flags, out);
        //function ends with the end of one file and the last used element of the other
    }
    while (inputFile1.peek() != EOF) { // file2 is checked, file1 not
        getline(inputFile1, one);
        print_1(one, out, flags);
    }
    while (inputFile2.peek() != EOF) { // file1 is checked, file2 not
        getline(inputFile2, two);
        print_2(two, out, flags);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4 && argc != 3) {
        std::cout << "Wrong amount of arguments. Provide filenames as arguments. See example below:\n";
        std::cout << "comm flag File1 File2" << std::endl;
        return 0;
    }
    std::ifstream inputFile1(argv[argc - 2]);
    if (!inputFile1.good()) {
        std::cout << "Can't open source file " << argv[argc - 2] << ". Verify that the file exists." << std::endl;
        return 0;
    }
    std::ifstream inputFile2(argv[argc - 1]);
    if (!inputFile2.good()) {
        std::cout << "Can't open source file " << argv[argc - 1] << ". Verify that the file exists." << std::endl;
        return 0;
    }
    std::vector<bool> flags = assign_f(argc,
                                       argv[1]); // I could use "bool flags[]", but it's less comfortable in functions
    compare(inputFile1, inputFile2, flags, std::cout);
    return 0;
}
