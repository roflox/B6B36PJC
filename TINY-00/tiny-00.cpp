#include "tiny-00.hpp"

#include <ostream>
#include <sstream>
// don't forget to include appropriate headers

void write_stats(std::vector<double> const& data, std::ostream& out) {
    double max, min, avrg;
    avrg = 0;
    max = data[0];
    min = data[0];
    for(double i: data){
        if(max < i ){
            max = i;
        }
        if(min > i){
            min = i;
        }
        avrg += i;
    }
    avrg /= data.size();
    std::stringstream sstream;
    sstream.setf(std::ios::fixed);
    sstream.precision(2);
    sstream << "min: "<<min<<"\nmax: "<<max<<"\nmean: "<<avrg<<"\n";
    out << sstream.str();
}