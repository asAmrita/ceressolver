#include "ceres/ceres.h"
#include "glog/logging.h"
using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;
// A templated cost functor that implements the residual r = 10 -
// x. The method operator() is templated so that we can then use an
// automatic differentiation wrapper around it to generate its
// derivatives.
struct CostFunctor {
  template <typename T> bool operator()(const T* const x1,const T* const x2, T* residual) const {
    residual[0] = (20.0)*x1[0] + (2.0)*x2[0];
    return true;
  }
};
int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);
  // The variable to solve for with its initial value. It will be
  // mutated in place by the solver.
  double x1 = 1;
double x2=2;
  const double initial_x1 = x1;
const double initial_x2 = x2;
  // Build the problem.
  Problem problem;
  // Set up the only cost function (also known as residual). This uses
  // auto-differentiation to obtain the derivative (jacobian).
  CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctor,1, 1, 1>(new CostFunctor);
  problem.AddResidualBlock(cost_function, NULL, &x1 ,&x2);
  
  // Run the solver!
  Solver::Options options;
  options.minimizer_progress_to_stdout = true;
  Solver::Summary summary;
  Solve(options, &problem, &summary);
  std::cout << summary.BriefReport() << "\n";
  std::cout << "x1 : " << initial_x1
            << " -> " << x1 << "\n";
  std::cout << "x2 : " << initial_x2 
            << " -> " << x2<< "\n";
  return 0;
}
