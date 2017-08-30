#include<iostream>
#include "common/BALProblem.h"
#include "common/BundleParams.h"
#include "SnavelyReprojectionError.h"
using namespace std;
using namespace ceres;


void BuildProblem(BALProblem *bal_problem, Problem *problem, const BundleParams &params) {
    const int point_block_size = bal_problem->point_block_size();
    const int camera_block_size = bal_problem->camera_block_size();
    double *points = bal_problem->mutable_points();
    double *cameras = bal_problem->mutable_cameras();
//    observations is 2*num_observations long array observations
//    [u_1,u_2,...,u_n], where each u_i is two dimensional, the x and y position of the observation.
    const double *observations = bal_problem->observations();

    for (int i = 0; i < bal_problem->num_observations(); ++i) {
        CostFunction *cost_function;
//        each residual block takes a point and camera as input
//        and outputs a 2 dimensional residual
        cost_function = SnavelyReprojectionError::Create(observations[2 * i + 0], observations[2 * i + 1]);
//        if enabled use Huber's loss function
        LossFunction *loss_function = params.robustify ? new HuberLoss(1.0) : nullptr;
//        each observation corresponds to a pair of a camera and a point
//        which are identified by camera_index()[i] and point_index()[i] respectively
        double *camera = cameras + camera_block_size * bal_problem->camera_index()[i];
        double *point = points + point_block_size * bal_problem->point_index()[i];

        problem->AddResidualBlock(cost_function, loss_function, camera, point);
    }
}


/**
 * 本程序演示了后端ceres bundle
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv) {

    return 0;
}