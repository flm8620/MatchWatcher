#pragma once
#include <map>
#include <QPoint>
#include <utility>
#include <vector>
class ImageScene
{
    std::vector<std::string> index_to_image_file_;
    std::vector<std::vector<AbstractFeature>> image_to_features_;
    std::vector<std::map<int, std::vector<std::pair<int, int>>>> image1_image2_matches_;
public:
    ImageScene() = default;
    ImageScene(
        std::vector<std::string> index_to_image_file,
        std::vector<std::vector<AbstractFeature>> image_to_features,
        std::vector<std::map<int, std::vector<std::pair<int, int>>>> image1_image2_matches)
        :
        index_to_image_file_(std::move(index_to_image_file)),
        image_to_features_(std::move(image_to_features)),
        image1_image2_matches_(std::move(image1_image2_matches))
    {

    }

    void setScene(const std::vector<std::string>& index_to_image_file,
        const std::vector<std::vector<AbstractFeature>>& image_to_features,
        const std::vector<std::map<int, std::vector<std::pair<int, int>>>>& image1_image2_matches) {
        this->index_to_image_file_ = index_to_image_file;
        this->image_to_features_ = image_to_features;
        this->image1_image2_matches_ = image1_image2_matches;
    }

    const auto& IndexToImageFile() const { return index_to_image_file_; }
    const auto& ImageToFeatures() const { return image_to_features_; }
    const auto& Image1Image2Matches() const { return image1_image2_matches_; }

};