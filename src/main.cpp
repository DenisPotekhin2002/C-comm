#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

const size_t colNum = 3;

std::vector<bool> assign_f(int n, const std::string &opt) { //parse flags for columns
    std::vector<bool> flags(n, true);
    if (n == 4) {
        assert(opt[0] == '-');
        for (size_t i = 1; i < opt.size(); i++) {
            flags[opt[i] - '1'] = false;
        }
    }
    return flags;
}

void print(std::string &str, std::ostream &out, const std::vector<bool> &flags, size_t num) { //first column
    if (!flags[num]) {
        return;
    }
    for (size_t i = 0; i < num; i++) {
        if (flags[i]) {
            out << "\t";
        }
    }
    out << str << std::endl;
}

void check_both(std::ifstream &inputFile1, std::ifstream &inputFile2, std::string &first, std::string &second,
                const std::vector<bool> &flags, std::ostream &out) {
    while (true) {
        while (second < first) { // read lines from 2, while less than first
            print(second, out, flags, 1);
            if (inputFile2.peek() != EOF) { // next line of file2, if exists
                getline(inputFile2, second);
            } else {
                print(first, out, flags, 0); // first is read, but not yet written
                return;
            }
        }
        while (first < second) { // read lines from 1, while less than second
            print(first, out, flags, 0);
            if (inputFile1.peek() != EOF) { // next line of file1, if exists
                getline(inputFile1, first);
            } else {
                print(second, out, flags, 1); // second is read, but not yet written
                return;
            }
        }
        while (first == second) {
            print(first, out, flags, 2);
            if (inputFile1.peek() != EOF && inputFile2.peek() != EOF) { // next line of each file, if exist
                getline(inputFile1, first);
                getline(inputFile2, second);
            } else {
                return;
            }
        }
    }
    //check_both(inputFile1, inputFile2, first, second, flags, out);
    //if you don't like loop "while(true)", this can be used instead
}

void compare(std::ifstream &inputFile1, std::ifstream &inputFile2, const std::vector<bool> &flags, std::ostream &out) {
    std::string first;
    std::string second;
    if (inputFile1.peek() != EOF && inputFile2.peek() != EOF) {
        getline(inputFile1, first);
        getline(inputFile2, second);
        check_both(inputFile1, inputFile2, first, second, flags, out);
        //function ends with the end of first file and the last used element of the other
    }
    while (inputFile1.peek() != EOF) { // file2 is checked, file1 not
        getline(inputFile1, first);
        print(first, out, flags, 0);
    }
    while (inputFile2.peek() != EOF) { // file1 is checked, file2 not
        getline(inputFile2, second);
        print(second, out, flags, 1);
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
    std::vector<bool> flags = assign_f(argc, argv[1]);
    // I could use "bool flags[]", but it's less comfortable in functions
    compare(inputFile1, inputFile2, flags, std::cout);
    return 0;
}
