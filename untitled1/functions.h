//
// Created by Matheus on 19/05/2023.
//

#ifndef UNTITLED1_FUNCTIONS_H
#define UNTITLED1_FUNCTIONS_H

#endif //UNTITLED1_FUNCTIONS_H

int vowels(const std::string key){
    int numVowels = 0;
    for (char c : key){
        c = tolower(c);
        if (c == 'a' | c == 'e' | c == 'i' | c == 'o' | c == 'u') numVowels += 1;
    }
    return numVowels;
}