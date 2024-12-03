#include <bits/stdc++.h>
using namespace std;

class BigInt {
private:
    vector<int> digits;
    bool isNegative;

    void removeLeadingZeros() {
        while (digits.size() > 1 && digits.back() == 0) {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0) {
            isNegative = false; 
        }
    }

    static BigInt add(const BigInt& a, const BigInt& b) {
        if (a.isNegative == b.isNegative) {
            BigInt result;
            result.isNegative = a.isNegative;
            int carry = 0, sum;
            size_t max_size = max(a.digits.size(), b.digits.size());
            for (size_t i = 0; i < max_size || carry; ++i) {
                sum = carry;
                if (i < a.digits.size()) sum += a.digits[i];
                if (i < b.digits.size()) sum += b.digits[i];
                result.digits.push_back(sum % 10);
                carry = sum / 10;
            }
            return result;
        }
        return a.isNegative ? subtract(b, -a) : subtract(a, -b);
    }

    static BigInt subtract(const BigInt& a, const BigInt& b) {
        if (a.isNegative != b.isNegative) {
            return a + (-b);
        }

        if (a.isNegative) {
            return -subtract(-b, -a);
        }

        if (a < b) {
            return -subtract(b, a);
        }

        BigInt result;
        result.isNegative = false;
        int carry = 0, diff;
        for (size_t i = 0; i < a.digits.size(); ++i) {
            diff = a.digits[i] - carry - (i < b.digits.size() ? b.digits[i] : 0);
            if (diff < 0) {
                diff += 10;
                carry = 1;
            } else {
                carry = 0;
            }
            result.digits.push_back(diff);
        }
        result.removeLeadingZeros();
        return result;
    }

    static BigInt multiply(const BigInt& a, const BigInt& b) {
        BigInt result;
        result.digits.resize(a.digits.size() + b.digits.size(), 0);
        result.isNegative = a.isNegative != b.isNegative;
        
        for (size_t i = 0; i < a.digits.size(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < b.digits.size() || carry; ++j) {
                long long cur = result.digits[i + j] +
                                a.digits[i] * 1LL * (j < b.digits.size() ? b.digits[j] : 0) + carry;
                result.digits[i + j] = cur % 10;
                carry = cur / 10;
            }
        }
        result.removeLeadingZeros();
        return result;
    }

    static pair<BigInt, BigInt> divide(const BigInt& a, const BigInt& b) {
        if (b == BigInt(0)) {
            throw invalid_argument("Division by zero");
        }

        BigInt quotient, remainder;
        BigInt abs_a = a.abs(), abs_b = b.abs();

        for (int i = a.digits.size() - 1; i >= 0; --i) {
            remainder = remainder * 10 + a.digits[i];
            int div = 0;
            while (remainder >= abs_b) {
                remainder -= abs_b;
                ++div;
            }
            quotient.digits.push_back(div);
        }
        reverse(quotient.digits.begin(), quotient.digits.end());
        quotient.removeLeadingZeros();
        quotient.isNegative = a.isNegative != b.isNegative;

        if (quotient.digits.empty()) {
            quotient.digits.push_back(0);
            quotient.isNegative = false;
        }

        remainder.isNegative = a.isNegative;
        remainder.removeLeadingZeros();
        return { quotient, remainder };
    }

public:
    BigInt() : isNegative(false) {}

    BigInt(const string& str) {
        if (str.empty()) {
            throw invalid_argument("Invalid input string");
        }

        isNegative = str[0] == '-';
        for (int i = str.size() - 1; i >= isNegative; --i) {
            if (!isdigit(str[i])) {
                throw invalid_argument("Invalid input string");
            }
            digits.push_back(str[i] - '0');
        }
        removeLeadingZeros();
    }

    BigInt(int num) {
        isNegative = num < 0;
        if (num == 0) {
            digits.push_back(0);
        } else {
            num = std::abs(num);
            while (num > 0) {
                digits.push_back(num % 10);
                num /= 10;
            }
        }
    }

    BigInt(long long num) {
        isNegative = num < 0;
        if (num == 0) {
            digits.push_back(0);
        } else {
            num = std::abs(num);
            while (num > 0) {
                digits.push_back(num % 10);
                num /= 10;
            }
        }
    }

    BigInt(const string& binStr, bool isBinary) {
        if (!isBinary) {
            throw invalid_argument("Second argument must be true for binary input");
        }
        isNegative = false;
        BigInt base(1);
        BigInt value(0);
        for (int i = binStr.size() - 1; i >= 0; --i) {
            if (binStr[i] == '1') {
                value += base;
            } else if (binStr[i] != '0') {
                throw invalid_argument("Invalid binary input string");
            }
            base *= 2;
        }
        *this = value;
    }

    string toBinaryString() const {
        if (*this == BigInt(0)) {
            return "0";
        }

        BigInt num = *this;
        string binary;
        BigInt two(2);

        while (num > BigInt(0)) {
            auto divmod = divide(num, two);
            binary.push_back((divmod.second == BigInt(1)) ? '1' : '0');
            num = divmod.first;
        }

        reverse(binary.begin(), binary.end());
        return binary;
    }

    BigInt operator-() const {
        BigInt result = *this;
        result.isNegative = !isNegative;
        return result;
    }

    BigInt& operator+=(const BigInt& other) {
        *this = *this + other;
        return *this;
    }

    BigInt& operator-=(const BigInt& other) {
        *this = *this - other;
        return *this;
    }

    BigInt& operator*=(const BigInt& other) {
        *this = *this * other;
        return *this;
    }

    BigInt& operator/=(const BigInt& other) {
        *this = *this / other;
        return *this;
    }

    BigInt& operator%=(const BigInt& other) {
        *this = *this % other;
        return *this;
    }

    friend BigInt operator+(const BigInt& a, const BigInt& b) {
        return add(a, b);
    }

    friend BigInt operator-(const BigInt& a, const BigInt& b) {
        return subtract(a, b);
    }

    friend BigInt operator*(const BigInt& a, const BigInt& b) {
        return multiply(a, b);
    }

    friend BigInt operator/(const BigInt& a, const BigInt& b) {
        return divide(a, b).first;
    }

    friend BigInt operator%(const BigInt& a, const BigInt& b) {
        return divide(a, b).second;
    }

    BigInt abs() const {
        BigInt result = *this;
        result.isNegative = false;
        return result;
    }

    friend bool operator<(const BigInt& a, const BigInt& b) {
        if (a.isNegative != b.isNegative) {
            return a.isNegative;
        }

        if (a.digits.size() != b.digits.size()) {
            return a.isNegative ? a.digits.size() > b.digits.size() : a.digits.size() < b.digits.size();
        }

        for (int i = a.digits.size() - 1; i >= 0; --i) {
            if (a.digits[i] != b.digits[i]) {
                return a.isNegative ? a.digits[i] > b.digits[i] : a.digits[i] < b.digits[i];
            }
        }
        return false;
    }

    friend bool operator>(const BigInt& a, const BigInt& b) {
        return b < a;
    }

    friend bool operator<=(const BigInt& a, const BigInt& b) {
        return !(a > b);
    }

    friend bool operator>=(const BigInt& a, const BigInt& b) {
        return !(a < b);
    }

    friend bool operator==(const BigInt& a, const BigInt& b) {
        return a.isNegative == b.isNegative && a.digits == b.digits;
    }

    friend bool operator!=(const BigInt& a, const BigInt& b) {
        return !(a == b);
    }

    friend ostream& operator<<(ostream& os, const BigInt& number) {
        if (number.isNegative) {
            os << '-';
        }
        for (int i = number.digits.size() - 1; i >= 0; --i) {
            os << number.digits[i];
        }
        return os;
    }

    friend istream& operator>>(istream& is, BigInt& number) {
        string str;
        is >> str;
        number = BigInt(str);
        return is;
    }
};

int main() {
    BigInt num1("12345624234789123453423498294823948246789");
    BigInt num2("9876233423423424442344234342423454321987654321");

    BigInt sum = num1 + num2;
    BigInt diff = num1 - num2;
    BigInt prod = num1 * num2;
    BigInt quot = num2 / num1;
    BigInt rem = num2 % num1;

    cout << "Sum: " << sum << endl;
    cout << "Difference: " << diff << endl;
    cout << "Product: " << prod << endl;
    cout << "Quotient: " << quot << endl;
    cout << "Remainder: " << rem << endl;

    BigInt num3(12345);
    BigInt num4(987654321987654321LL);

    cout << "BigInt from int: " << num3 << endl;
    cout << "BigInt from long long: " << num4 << endl;

    BigInt binaryNum("1101", true);
    cout << "BigInt from binary: " << binaryNum << endl;
    cout << "Binary string of " << binaryNum << ": " << binaryNum.toBinaryString() << endl;

    return 0;
}
