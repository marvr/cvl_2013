
#include "nft_application.hpp"

nft_application::nft_application():
	object_aquisition(false),
	capt_frame(false),
	show_status_info(false),
	track(false)
{
	detector = new detection();
	tracker = new tracking();
};

//changed by hongyi
void nft_application::process_frame(cv::Mat& rgb, cv::Mat& gray){
	// Step 1: Detect features, look for object, determine homography
	std::vector<cv::KeyPoint> new_keypoints;
	cv::Mat homography;

    if (capt_frame){
    	detector->extract_and_add_raw_features(rgb, gray);
    	capt_frame = false;
    	return;
    }

    if(detector->detect(gray)){
    	// We have a homography -> warp and show rectangle or opengl model
    	detector->warp_rectangle(rgb);
    }

	//detector->show_features(rgb, new_keypoints);

	if(show_status_info){
		detector->overlay_status_info(rgb);
	}

	if(object_aquisition){
		detector->show_target_rectangle(rgb, cv::Point2i(480,320), cv::Point2i(240,160));
	}


};

void nft_application::capture_frame(){
	capt_frame = true;
};

void nft_application::set_feature(int feature){
	// set feature type

};

void nft_application::show_info(bool status){
	show_status_info = status;
};

void nft_application::object_acquisition(bool status){
	object_aquisition = status;
};
