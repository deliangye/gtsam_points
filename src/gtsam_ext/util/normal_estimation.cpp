#include <gtsam_ext/util/normal_estimation.hpp>

#include <Eigen/Eigen>
#include <gtsam_ext/util/covariance_estimation.hpp>

namespace gtsam_ext {

std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>>
estimate_normals(const Eigen::Vector4d* points, const Eigen::Matrix4d* covs, int num_points, int k_neighbors) {
  std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> normals(num_points, Eigen::Vector4d::Zero());
  for (int i = 0; i < num_points; i++) {
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> eig;
    eig.computeDirect(covs[i].block<3, 3>(0, 0));
    normals[i].head<3>() = eig.eigenvectors().col(0);

    if (points[i].dot(normals[i]) > 1.0) {
      normals[i] = -normals[i];
    }
  }

  return normals;
}

std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> estimate_normals(const Eigen::Vector4d* points, int num_points, int k_neighbors) {
  auto covs = estimate_covariances(points, num_points, k_neighbors);
  return estimate_normals(points, covs.data(), num_points, k_neighbors);
}

std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> estimate_normals(
  const std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>>& points,
  const std::vector<Eigen::Matrix4d, Eigen::aligned_allocator<Eigen::Matrix4d>>& covs,
  int k_neighbors) {
  //
  return estimate_normals(points.data(), covs.data(), points.size(), k_neighbors);
}

std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> estimate_normals(
  const std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>>& points,
  int k_neighbors) {
  //
  auto covs = estimate_covariances(points, k_neighbors);
  return estimate_normals(points, covs);
}

}  // namespace gtsam_ext