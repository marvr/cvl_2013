#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>

#include "detection.hpp"

detection::detection():
	initial_keypoints(0),
	initial_descriptors(0),
	raw_descriptors(0),
	detector(250),
	matcher(cv::NORM_HAMMING, true)
{};

detection::~detection() {
};


void detection::extract_and_add_raw_features(cv::Mat& img){
	// img should be a ROI-Matrix representing the center of the frame targeted for object recognition
	// add descriptors to the initial raw set.
	// call setup_initial_features to calculate the initial set of features from the raw collection
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;

	detector.detect(img, keypoints);
	detector.compute(img, keypoints, descriptors);

	raw_descriptors.push_back(descriptors);
};

void detection::setup_initial_features(){
	// calculate the set of initial features (descriptors only) from the raw feature sets
	std::vector<std::vector<cv::DMatch> > matches;
	if(raw_descriptors.size() == 0){
		//not enough frames for matching
	}
	else if(raw_descriptors.size() > 1){
		initial_descriptors.push_back(raw_descriptors[0]);
	}
	else {
		initial_descriptors.reserve(2);
		// match and filter
		matcher.knnMatch( raw_descriptors[0], raw_descriptors[1], matches, 2 );
		for( unsigned int i = 0; i < matches.size(); i++ )
			{
				cv::DMatch m1 = matches[i][0];
				cv::DMatch m2 = matches[i][1];
				if(m1.distance < 0.6* m2.distance){
					//cv::Mat& v1, v2;
					// TODO single feature extraction from descriptor matrix :/
					//v1 = initial_descriptors[0];
					//v1.push_back(m1.trainIdx);
					//v2 = initial_descriptors[1];
					//v2.push_back(m1.queryIdx);
				}
			}
	}
};


std::vector<cv::KeyPoint> detection::track(cv::Mat& img){
	// try to find and track the initial features in the given image
	std::vector<cv::KeyPoint> keypoints_new, result;
	cv::Mat descriptors_new;
	std::vector<std::vector<cv::DMatch> > matches;

	detector.detect(img, keypoints_new);
	detector.compute(img, keypoints_new, descriptors_new);

	matcher.knnMatch( initial_descriptors[0], descriptors_new, matches, 2);

	for( unsigned int i = 0; i < matches.size(); i++ )
	{
		cv::DMatch m1 = matches[i][0];
		cv::DMatch m2 = matches[i][1];
		if(m1.distance < 0.6* m2.distance){
			result.push_back(keypoints_new[m1.trainIdx]);
	        }
	    }
	return result;
};

void detection::add_target_rectangle(cv::Mat& img, cv::Point2i top_left, cv::Point2i bottom_right){
	//add targeting rectangle for initial acquisition
	cv::rectangle(img, top_left, bottom_right, cv::Scalar(100,100,100,100), 2);
};
