#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>    // std::priority_queue

#include <iomanip>  // std::setprecision
#include <cstring>
#include <climits>  // INT_MAX INT_MIN

#include <string_view>

class Stat { // Statistics
public:
    explicit Stat() :_min(INT_MAX), _max(INT_MIN), _sum(0), _cnt(0) {}
    void take_in(int val) {
        if (_min > val) {
            _min = val;
        }
        if (val > _max) {
            _max = val;
        }
        _sum += val;
        ++_cnt;
    }

    double getAvg() const {
        return static_cast<double>(_sum) / _cnt;
    }

    int _min, _max, _sum, _cnt;
};

std::ostream& operator<< (std::ostream& os, const Stat& stat) {
    os << stat._min << ".0, "
        << stat._max << ".0, "
        << std::fixed << std::setprecision(1) << stat.getAvg() << ", "
        << stat._cnt << ".0" << std::endl;
    return os;
} // Stat

class Agg {  // Aggregation
public:
    ~Agg() = default;
    Agg():pq(compare_plate_fine){}

    void take_in(const std::string&& key_zip_date, int fine) {
        auto iter = agg_body.find(key_zip_date);
        if (iter == agg_body.end()) {
            Stat stat;
            stat.take_in(fine);
            agg_body.insert(std::make_pair(key_zip_date, stat));
        }
        else {
            iter->second.take_in(fine);
        }
    }

    void take_in2(const std::string&& plate_id, int fine) {
        auto iter = agg_body2.find(plate_id);
        if (iter == agg_body2.end()) {
            agg_body2.insert(std::make_pair(plate_id, fine));
        }
        else {
            iter->second = iter->second + fine;
        }
    }

    void get_top(int K = 10) {
        for (auto& p : agg_body2) {
            pq.push(p);
            if (pq.size() > K) {
                pq.pop();
            }
        }
    }
    friend std::ostream& operator<< (std::ostream& os, Agg& agg);

private:
    std::map<std::string, Stat> agg_body; // key: zip+date, value: Stat
    std::unordered_map<std::string, int> agg_body2; // key: plate_id, value: fine 
    using Plate_fine = std::pair<std::string, int>;
    static auto constexpr compare_plate_fine = [](const Plate_fine& a, const Plate_fine& b) {
        return a.second > b.second;
    };
    mutable std::priority_queue<Plate_fine, std::vector<Plate_fine>, decltype(compare_plate_fine)> pq;
};

std::ostream& operator<< (std::ostream& os, Agg& agg) {
    for (const auto& p : agg.agg_body) {
        os << std::string_view(&p.first[0], 5) << ", " // zip
            << std::string_view(&p.first[0] + 5, 10) << ", " // date
            << p.second;  // Statistics
    }
    os << "Plate Id,  total fine" <<std::endl;
    while (agg.pq.size()) {
        std::string_view first(agg.pq.top().first);
        os << first << ", " << agg.pq.top().second << std::endl;
        agg.pq.pop();
    }
    return os;
} // Agg

class Csv {
public:
    Csv() = default;
    ~Csv() = default;
    std::string_view operator[](std::size_t i) const {
        return std::string_view(&_line[_sep_pos[i] + 1], _sep_pos[i + 1] - _sep_pos[i] - 1);
    }

    friend std::ostream& operator<< (std::ostream& os, Csv& csv);

    void get_next_line(std::istream& stream) {
        std::getline(stream, _line);

        if (total == 0) { // skip the header
            ++total;
            return;
        }
        if (_line.empty()) return;
        ++total;

        extract_sep_pos();
        process_line();
    }

    void extract_sep_pos() { // extract separator position
        _sep_pos.clear();
        _sep_pos.emplace_back(-1);
        for (std::string::size_type i = 0; i < _line.size(); ++i) {
            if (_line[i] == ',') {
                _sep_pos.emplace_back(i);
            }
        }
        _sep_pos.emplace_back(_line.size());
    }

    void process_line() {
        /*                  0              1     2             3        4
           anon_ticket_number,issue_datetime,state,anon_plate_id,division,
                  5              6    7              8   9   0   1        2
           location,violation_desc,fine,issuing_agency,lat,lon,gps,zip_code
        */
        std::string key_zip_date(this->operator[](12));   // zip
        // std::string_view datetime = this->operator[](1);  // datetime
        std::string date(std::string_view(&_line[_sep_pos[1] + 1], 10));  // 1st 10 char of datetime
        // len(yyyy-MM-dd) == 10

        std::string plate_id(this->operator[](3));

        key_zip_date += date;

        int fine = -1;
        try {
            fine = std::stoi(std::string(this->operator[](7)));
        }
        catch (const std::invalid_argument& e) {
            std::cout << "# Invalid argument, cannot convert to Integer # \n" << _line << ", \n"
                << "# error from " << e.what() << std::endl;
            return;
        }

        _agg.take_in(std::move(key_zip_date), fine);
        _agg.take_in2(std::move(plate_id), fine);
    }
    Agg& get_agg() { return _agg; }
private:
    std::string _line;
    std::vector<int> _sep_pos; // store the index of seperator ','
    Agg _agg;
    std::size_t total = 0;
};

std::ostream& operator<< (std::ostream& os, Csv& csv) {
    std::cout << "Zip Code, Date, Min Fine, Max Fine, Avg Fine, Count of Tickets" << std::endl;
    os << csv.get_agg();
    return os;
}

std::istream& operator>> (std::istream& fs, Csv& csv) {
    csv.get_next_line(fs);
    return fs;
} // Csv

int process_csv_file(const char* file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
				if (file.rdstate() & std::ios::failbit) 
					std::cerr<< "Failbit is set" << std::endl;
        return 1;
    }

    Csv csv;

    while (file >> csv) {}
    Agg& p = csv.get_agg();
    p.get_top();

    std::cout << csv;
    return 0;
}

int main(int argc, char* argv[]) {
    // std::cout << "current working path is" << std::filesystem::current_path() << std::endl;
    if (argc == 1) {
        // process_csv_file("../data/data2.csv");
        process_csv_file("/home/hwd/prj/cpp-interview/data/data2.csv");
    }
    else if (argc == 2) {
        if (std::strcmp(argv[1], "test") == 0) {
            // Tests tests;
            // tests.RunTests();
        }
        else {
            process_csv_file(argv[1]);
        }
    }
    else { // argc > 2
        std::cerr << "Invalid arguments" << std::endl;
    }

    return 0;
}

