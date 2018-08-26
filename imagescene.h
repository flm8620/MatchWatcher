#pragma once
#include <map>
#include <utility>
#include <vector>

struct AbstractFeature
{
    QPointF pos;
    float scale;
    float orient;
};

struct ImageInfo
{
    std::string image_file, sift_file, mat_file;
    float focal, qw, qx, qy, qz, cx, cy, cz, r;
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