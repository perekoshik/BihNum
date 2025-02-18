
#include <iostream>
#include <string>

int main (){
  std::string str="We think in generalities, but we live in details.";

  std::string str2 = str.substr (3,5);     // "think"

  std::cout << str2;

  return 0;
}