#include <iostream>
#include <tuple>
#include <vector>

#include <aks.h>
#include <bignum.h>
#include <miller_rabin.h>
#include <soe.h>
#include <trial_division.h>

#include "utils.h"
#include "tests.h"

namespace
{
    void PrintMenu(const std::string& title, const std::vector<std::pair<std::string, std::function<void()>>>& options)
    {
        std::cout << std::endl << title << std::endl;
        std::cout << "Enter a number or (q) to quit" << std::endl;
        for (size_t i = 0; i != options.size(); i++)
            std::cout << std::to_string(i + 1) << ") " << options[i].first << std::endl;
    }

    void Menu(const std::string& title, const std::vector<std::pair<std::string, std::function<void()>>>& options)
    {
        PrintMenu(title, options);
        std::string input;
        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (input != "q")
        {
            int optionIndex = -1;
            try
            {
                optionIndex = std::stoi(input) - 1;
            }
            catch(const std::logic_error& err)
            {
                optionIndex = -1;
            }

            if (optionIndex >= 0 && optionIndex < options.size())
                options[optionIndex].second();
            else
                std::cout << "Wrong input." << std::endl;

            PrintMenu(title, options);
            std::cin >> input;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

int main(int argc, char *argv[])
{
    //TestBigNum();

    std::vector<std::pair<std::string, std::function<void()>>> menuOptions;
    menuOptions.push_back(std::make_pair<std::string, std::function<void()>>("Test AKS", [](){
        Prime::TestAsync([](const Prime::BigNum& num){return Prime::IsPrimeAKS(num);}, "AKS");}));
    menuOptions.push_back(std::make_pair<std::string, std::function<void()>>("Test AKS from file", [](){
        Prime::TestFromFileAsync([](const Prime::BigNum& num){return Prime::IsPrimeAKS(num);}, "AKS from file");}));

    menuOptions.push_back(std::make_pair<std::string, std::function<void()>>("Test Miller-Rabin", [](){
        Prime::TestAsync([](const Prime::BigNum& num){return Prime::IsPrimeMillerRabin(num);}, "Miller-Rabin");}));
    menuOptions.push_back(std::make_pair<std::string, std::function<void()>>("Test Miller-Rabin from file", [](){
        Prime::TestFromFileAsync([](const Prime::BigNum& num){return Prime::IsPrimeMillerRabin(num);}, "Miller-Rabin from file");}));

    menuOptions.push_back(std::make_pair<std::string, std::function<void()>>("Trial Division", [](){
        Prime::TestAsync([](const Prime::BigNum& num){return Prime::IsPrimeTrialDivision(num);}, "Trial Division");}));
    menuOptions.push_back(std::make_pair<std::string, std::function<void()>>("Trial Division from file", [](){
        Prime::TestFromFileAsync([](const Prime::BigNum& num){return Prime::IsPrimeTrialDivision(num);}, "Trial Division from file");}));

    Menu("Main menu", menuOptions);

    return 0;
}
