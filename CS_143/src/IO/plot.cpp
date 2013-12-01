// Compile it with:
//   g++ -o example-vector example-vector.cc -lboost_iostreams -lboost_system -lboost_filesystem

#include <vector>
#include <cmath>
#include <boost/tuple/tuple.hpp>

#include "gnuplot-iostream.h"

int main() {
	Gnuplot gp;
	std::vector<boost::tuple<double, double> > pts_A;
	// You can also use a separate container for each column if that would be better

//	for(double alpha=0; alpha<1; alpha+=1.0/24.0) {
//		double theta = alpha*2.0*3.14159;
//		pts_A.push_back(boost::make_tuple(
//			 cos(theta),
//			 sin(theta)));
//	}

	gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
	gp << "plot '-' with lines title 'pts_A'\n";
	gp.send1d(pts_A);
}
