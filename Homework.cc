#include <vector>
#include "ceres/ceres.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;
struct F1 {
  template <typename T> bool operator()(const T* const x1,
                                        
                                        T* residual) const {
    // f1 = x1 + 10 * x2;
    residual[0] = 10.0*x1[0] ;
    return true;
  }
};
struct F2 {
  template <typename T> bool operator()(
                                        const T* const x2,
                                        T* residual) const {
    // f1 = x1 + 10 * x2;
    residual[0] =   x2[0];
    return true;
  }
};

DEFINE_string(minimizer, "trust_region",
              "Minimizer type to use, choices are: line_search & trust_region");
int main(int argc, char** argv) {
  GFLAGS_NAMESPACE::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  double x1=2.0;
double x2=2.0;



  // Build the problem.
  
  Problem problem;
  // Add residual terms to the problem using the using the autodiff
  // wrapper to get the derivatives automatically. The parameters, x1 through
  // x4, are modified in place.
  problem.AddResidualBlock(new AutoDiffCostFunction<F1, 1, 1>(new F1),
                           NULL,&x1);
  problem.SetParameterLowerBound(&x1, 0, -1.0);
  problem.SetParameterUpperBound(&x1, 0, 1.0);
  problem.AddResidualBlock(new AutoDiffCostFunction<F2, 1, 1>(new F2),
                           NULL,&x2);
 problem.SetParameterLowerBound(&x2, 0, 0.5);
 problem.SetParameterUpperBound(&x2, 0, 2.0);
                           
  Solver::Options options;

 LOG_IF(FATAL, !ceres::StringToMinimizerType(FLAGS_minimizer,
                                              &options.minimizer_type))
      << "Invalid minimizer: " << FLAGS_minimizer
      << ", valid options are: trust_region and line_search.";
  options.max_num_iterations = 100;
  options.linear_solver_type = ceres::DENSE_QR;
  options.minimizer_progress_to_stdout = true;
  std::cout << "Initial x1 = " << x1
            << ", x2 = " << x2
            
            << "\n";
//std::cout << "x1 : " << initial_x1
           // << " -> " << final_x1 << "\n";
//std::cout << "x2 : " << initial_x2 
            //<< " -> " << final_x2<< "\n";
  // Run the solver!

  Solver::Summary summary;
  Solve(options, &problem, &summary);

  std::cout << summary.FullReport() << "\n";
  std::cout << "Final x1 = " << x1
            << ", x2 = " <<x2
           
            << "\n";


  return 0;
}
