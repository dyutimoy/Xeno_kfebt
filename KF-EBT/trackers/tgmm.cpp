#include "tgmm.h"

tGMM::tGMM(float dist_adj, float conf_adj)
{
    this->dist_adj = dist_adj;
    this->conf_adj = conf_adj;
}

void tGMM::init(cv::Mat& image, cv::Rect region){
    gmm.init(image, region.x, region.y, region.x + region.width, region.y + region.height);
    ratio = (float)region.height/(float)region.width;
    updateModel = false;
}

void tGMM::correctState(std::vector<float> st){
    this->state = st;
    gmm.position.height = st[2]*ratio;
    gmm.position.width = st[2];
    gmm.position.x = st[0] - gmm.position.width/2;
    gmm.position.y = st[1] - gmm.position.height/2;
}
//this needs to modified properly
void tGMM::track(){
    double confidenceGMM = 0;

    gmm.track(currentFrame, &confidenceGMM);


    // write state
    state.clear();
    state.push_back(gmm.position.x);
    state.push_back(gmm.postion.y);
    state.push_back(gmm.lastPosition.width);
      ///need to change this part
    this->stateUncertainty.clear();
    float penalityGMM = pow(dist_adj*fabs(state[0] - currentPredictRect[0])/((double)gmm.position.width),2)  +
                         pow(dist_adj*fabs(state[1] - currentPredictRect[1])/((double)gmm.position.height), 2);// +
                         //pow(dist_adj*fabs(state[2] - currentPredictRect[2])/(double)asms.lastPosition.width,2);
        ///need to decide confidenceGMM kya karu iska
    float uncertainty = 1e-4*exp(-3.5*(conf_adj*confidenceGMM - penalityGMM));
    stateUncertainty.push_back(uncertainty);
    stateUncertainty.push_back(uncertainty);
    stateUncertainty.push_back(uncertainty);
}

void tGMM::update(){
    //asms.update();
}

void tGMM::newFrame(cv::Mat &image, std::vector<float> predictRect){
    currentFrame = image;
    currentPredictRect = predictRect;
}

cv::Rect tGMM::getRect(){
    cv::Rect rect;
    rect.x = round(gmm.position.x);
    rect.y = round(gmm.position.y);
    rect.width = round(gmm.position.width);
    rect.height = round(gmm.position.height);
    return rect;
}
///why oo why ratio is constatnt
