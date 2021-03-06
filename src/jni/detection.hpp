#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>

#pragma once

class detection{
	// features
public:
	cv::FeatureDetector* gen_detector;
    cv::OrbFeatureDetector  detector;
    cv::BFMatcher matcher;
    cv::FlannBasedMatcher flann_matcher;
    std::vector<cv::KeyPoint> initial_keypoints;
    std::vector<cv::Mat> initial_descriptors;
    std::vector<cv::Mat> raw_descriptors;

    cv::Point2i previous_location;
    std::vector<int> pts_index;
    cv::Mat prev_img, orig_img;
    std::vector<cv::Point2f> last_pts, current_pts;
    int skipped_frames;
    bool redetection;

    cv::Mat homography;
    bool initialized;
    cv::Mat K, K_inv;

    //wire model house
    std::vector<cv::Point3f> house_vertices;
    std::vector<std::vector<int> > house_edges;
    //std::vector<std::tuple<int, int> > house_edges;

	// init & destructor
	detection();
	~detection();


	// functionality
	std::vector<cv::KeyPoint> detect_keypoints(cv::Mat& img);

	// setup feature tracking
	void extract_and_add_raw_features(cv::Mat& gr_img, cv::Mat& rgb_img);
	void setup_initial_features();

	//feature tracking
	// allow multiple trackings?
	bool detect(cv::Mat& img);

	void show_target_rectangle(cv::Mat& img, cv::Point2i top_left, cv::Point2i bottom_right);
	// for aquiring the raw features: the desired object to track should be in the center of the frame

	void show_features(cv::Mat& img, std::vector<cv::KeyPoint>& points);
	// displays some features in the image

	void overlay_status_info(cv::Mat& img);
	// overlay image with status info (number of key frames, number of features etc...)

	void  warp_rectangle(cv::Mat& img);
	//warp a smaller targent rectangle according to the current homography

	void set_feature(int idx);

	std::vector<cv::KeyPoint> non_max_suppression(std::vector<cv::KeyPoint> keypoints, int max_dist);
	// non-maximum suppression for feature detectors which lack this;
	// should probably be used for initial feature setup only

	void set_up_house();
};
