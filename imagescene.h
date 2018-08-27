#pragma once
#include <map>
#include <utility>
#include <vector>
#include <Eigen/Dense>
struct AbstractFeature
{
    QPointF pos;
    float scale;
    float orient;
};

inline Eigen::Matrix3d MatrixCross(const Eigen::Vector3d& v) {
    Eigen::Matrix3d M = Eigen::Matrix3d::Zero();
    M(0, 1) = -v[2];
    M(1, 0) = v[2];
    M(0, 2) = v[1];
    M(2, 0) = -v[1];
    M(1, 2) = -v[0];
    M(2, 1) = v[0];
    return M;
}

struct ImageInfo
{
    std::string image_file, sift_file, mat_file;
    int width, height;
    double focal, qw, qx, qy, qz, Cx, Cy, Cz, r;
    Eigen::Matrix3d getK() const {
        Eigen::Matrix3d K = Eigen::Matrix3d::Zero();
        K(0, 0) = K(1, 1) = focal;
        K(0, 2) = double(width) / 2.;
        K(1, 2) = double(height) / 2.;
        K(2, 2) = 1.;
        return K;
    }

    Eigen::Matrix3d getR() const {
        return Eigen::Quaterniond(qw, qx, qy, qz).toRotationMatrix();
    }

    Eigen::Vector3d getC() const {
        return { Cx,Cy,Cz };
    }

    Eigen::Vector3d getT() const {
        Eigen::Vector3d C = -getR() * getC();
        return C;
    }

    // x2^T * E * x1 == 0
    static Eigen::Matrix3d calcEssentialMatrix(const ImageInfo& im1, const ImageInfo& im2) {
        const Eigen::Matrix3d R1 = im1.getR();
        const Eigen::Matrix3d R2 = im2.getR();
        const Eigen::Vector3d C2 = im2.getC();
        const Eigen::Vector3d T1 = im1.getT();
        return R2 * R1.transpose()*MatrixCross(R1*C2 + T1);
    }

    static Eigen::Matrix3d calcFundamentalMatrix(const ImageInfo& im1, const ImageInfo& im2) {
        const Eigen::Matrix3d E = calcEssentialMatrix(im1, im2);
        const Eigen::Matrix3d K1 = im1.getK();
        const Eigen::Matrix3d K2 = im2.getK();
        return K2.transpose().inverse() * E * K1.inverse();
    }
};

class ImageScene
{
    std::vector<ImageInfo> images_;
    std::vector<std::vector<AbstractFeature>> image_to_features_;
    std::vector<std::map<int, std::vector<std::pair<int, int>>>> image1_image2_matches_;
public:
    ImageScene() = default;
    ImageScene(
        std::vector<ImageInfo> images,
        std::vector<std::vector<AbstractFeature>> image_to_features,
        std::vector<std::map<int, std::vector<std::pair<int, int>>>> image1_image2_matches)
        :
        images_(std::move(images)),
        image_to_features_(std::move(image_to_features)),
        image1_image2_matches_(std::move(image1_image2_matches))
    {

    }

    void setScene(const std::vector<ImageInfo>& images,
        const std::vector<std::vector<AbstractFeature>>& image_to_features,
        const std::vector<std::map<int, std::vector<std::pair<int, int>>>>& image1_image2_matches) {
        this->images_ = images;
        this->image_to_features_ = image_to_features;
        this->image1_image2_matches_ = image1_image2_matches;
    }

    const std::vector<ImageInfo>& Images() const { return images_; }
    const std::vector<std::vector<AbstractFeature>>& ImageToFeatures() const { return image_to_features_; }
    const std::vector<std::map<int, std::vector<std::pair<int, int>>>>& Image1Image2Matches() const { return image1_image2_matches_; }

};