
#include <algorithm> // For std::abs
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

class Fraction {
private:
  int numerator;
  int denominator;

  // Function to calculate the Greatest Common Divisor (GCD)
  int gcd(int a, int b) const {
    while (b != 0) {
      int temp = b;
      b = a % b;
      a = temp;
    }
    return a;
  }

  // Function to calculate the Least Common Denominator (LCD)
  int lcd(int a, int b) const { return abs(a * b) / gcd(a, b); }

  // Function to simplify the fraction
  void simplify() {
    int common_divisor = gcd(abs(numerator), abs(denominator));
    numerator /= common_divisor;
    denominator /= common_divisor;
    if (denominator < 0) {
      numerator = -numerator;
      denominator = -denominator;
    }
  }

public:
  // Default constructor
  Fraction() : numerator(0), denominator(1) {}

  // Constructor from a single integer (num/1)
  Fraction(int num) : numerator(num), denominator(1) { simplify(); }

  // Constructor with numerator and denominator
  Fraction(int num, int den) : numerator(num), denominator(den) {
    if (denominator == 0) {
      throw invalid_argument("Denominator cannot be zero.");
    }
    simplify();
  }

  // Overloaded addition operator
  Fraction operator+(const Fraction &other) const {
    int common_denominator = lcd(denominator, other.denominator);
    int adjusted_numerator1 = numerator * (common_denominator / denominator);
    int adjusted_numerator2 =
        other.numerator * (common_denominator / other.denominator);
    int result_numerator = adjusted_numerator1 + adjusted_numerator2;
    return Fraction(result_numerator, common_denominator);
  }

  // Overloaded subtraction operator
  Fraction operator-(const Fraction &other) const {
    int common_denominator = lcd(denominator, other.denominator);
    int adjusted_numerator1 = numerator * (common_denominator / denominator);
    int adjusted_numerator2 =
        other.numerator * (common_denominator / other.denominator);
    int result_numerator = adjusted_numerator1 - adjusted_numerator2;
    return Fraction(result_numerator, common_denominator);
  }

  // Overloaded multiplication operator
  Fraction operator*(const Fraction &other) const {
    int result_numerator = numerator * other.numerator;
    int result_denominator = denominator * other.denominator;
    return Fraction(result_numerator, result_denominator);
  }

  // Overloaded division operator
  Fraction operator/(const Fraction &other) const {
    if (other.numerator == 0) {
      throw invalid_argument(
          "Cannot divide by a fraction with zero numerator.");
    }
    int result_numerator = numerator * other.denominator;
    int result_denominator = denominator * other.numerator;
    return Fraction(result_numerator, result_denominator);
  }

  // Overloaded equality operator (==)
  bool operator==(const Fraction &other) const {
    return numerator == other.numerator && denominator == other.denominator;
  }

  // Overload output operator (<<) for printing fractions
  friend ostream &operator<<(ostream &os, const Fraction &frac) {
    os << frac.numerator << "/" << frac.denominator;
    return os;
  }

  // Function to read a fraction from a string
  static Fraction fromString(const string &str) {
    int num, den;
    char slash;
    istringstream iss(str);
    iss >> num >> slash >> den;
    if (den == 0) {
      throw invalid_argument("Denominator cannot be zero.");
    }
    return Fraction(num, den);
  }
};

// Function to process each line of input
void processLine(const string &line) {
  istringstream iss(line);
  string frac1_str, frac2_str;
  char operation;

  if (iss >> frac1_str >> operation >> frac2_str) {
    Fraction frac1 = Fraction::fromString(frac1_str);
    Fraction frac2 = Fraction::fromString(frac2_str);
    Fraction result(0, 1);

    switch (operation) {
    case '+':
      result = frac1 + frac2;
      break;
    case '-':
      result = frac1 - frac2;
      break;
    case '*':
      result = frac1 * frac2;
      break;
    case '/':
      result = frac1 / frac2;
      break;
    default:
      cerr << "Invalid operation: " << operation << endl;
      return;
    }
    cout << "Result of " << frac1 << " " << operation << " " << frac2 << " = "
         << result << endl;
  } else {
    cerr << "Invalid input format: " << line << endl;
  }
}

int main() {
  ifstream inputFile("input.dat");
  if (!inputFile) {
    cerr << "Error opening file." << endl;
    return 1;
  }

  string line;
  while (getline(inputFile, line)) {
    processLine(line);
  }

  inputFile.close();
  return 0;
}
