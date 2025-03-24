#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <random>

using namespace std;

const int CASE_AMOUNT = 50000;
const map<string, string> files = {
    {"cash", "data/3_mo_TBill_rate_1926-2013_pct.txt"},
    {"bonds", "data/10-yr_TBond_returns_1926-2013_pct.txt"},
    {"stocks", "data/SP500_returns_1926-2013_pct.txt"},
    {"inflation", "data/annual_infl_rate_1926-2013_pct.txt"}
};
const int RECORDS_START = 1926;
const int RECORDS_END = 2013;

double total;
double cashp, bondp, stockp;
double expenses;
int minyears, maxyears, probyears;
tuple<double, double, double> distribution;

double get_input(string prompt) {
    cout << prompt;
    double res = -1;
    bool valid = false;
    while (!valid) {
        try {
            cin >> res;
            if (cin.fail()) {
                cout << "Invalid input. Please try again. \n";
                cout << prompt;
                // clear cin error state
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            valid = true;
        } catch (...) {
            cout << "Invalid input. Please try again. \n";
            cout << prompt;
        }
    }
    cout << "\n";
    return res;
}

int main() {
    map<string, vector<double>> records;
    for (pair<string, string> file : files) {
        records[file.first] = {};
        ifstream input(file.second);
        string s;
        while (getline(input, s)) {
            double percent = atoi(s.c_str());
            records[file.first].push_back(percent / 100);
        }
        input.close();
    }
    
    total = get_input("Input your total retirement savings: ");
    cashp = get_input("Input the percentage of the savings stored as cash: ");
    bondp = get_input("Input the percentage of the savings stored in bonds: ");
    stockp = get_input("Input the percentage of the savings stored in stocks: ");
    if (cashp + bondp + stockp != 100.0) {
        throw std::invalid_argument("The percentages of your investments do not add up!");
    }
    distribution = {total * cashp / 100, total * bondp / 100, total * stockp / 100};

    expenses = get_input("Input your annual expenses: ");
    minyears = get_input("Input minimum years in retirement: ");
    maxyears = get_input("Input maximum years in retirement: ");
    probyears = get_input("Input most-likely amount of years in retirement: ");
    if (probyears > maxyears || probyears < minyears) {
        throw std::invalid_argument("Most likely years must fall between minimum and maximum years!");
    }
    vector<double> i = {(double)minyears, (double)probyears, (double)maxyears}; // double to prevent floating point exception
    vector<double> weights = {0, 1, 0};
    piecewise_linear_distribution<double> dist (i.begin(), i.end(), weights.begin());
    uniform_int_distribution<mt19937::result_type> period (RECORDS_START, RECORDS_END);
    random_device rd;
    mt19937 gen(rd());

    vector<tuple<double, double, double>> outcomes;
    outcomes.reserve(CASE_AMOUNT);
    int bankrupt = 0;
    for (int c = 0; c < CASE_AMOUNT; c ++) {
        tuple<double, double, double> money = distribution;
        int start = period(gen);
        int years = dist(gen); // years geometry dash reference lol
        int end = start + years;

        double withdrawal = expenses; 
        for (int year = start; year <= end; year ++) {
            int index = (year - RECORDS_START) % (RECORDS_END - RECORDS_START + 1);
            if (year != start) {
                withdrawal = (1 + records["inflation"][index]) * withdrawal;
            }
            money = {get<0> (money) - withdrawal * cashp / 100,
            get<1> (money) - withdrawal * bondp / 100,
            get<2> (money) - withdrawal * stockp / 100};
            money = {get<0> (money) * (1 + records["cash"][index]),
            get<1> (money) * (1 + records["bonds"][index]),
            get<2> (money) * (1 + records["stocks"][index])};
            if (get<0> (money) + get<1> (money) + get<2> (money) < 1.0) {
                money = {0, 0, 0};
                break;
            }
        }
        outcomes.push_back({start, end, 
        get<0> (money) + get<1> (money) + get<2> (money)});
        // cout << (int)get<2> (outcomes.back()) << "\n";
        if (get<2> (outcomes.back()) == 0) bankrupt ++;
    }
    cout << "----\n";
    cout << "Probability of bankruptcy: " << (double)bankrupt / (double)CASE_AMOUNT << "\n";
}