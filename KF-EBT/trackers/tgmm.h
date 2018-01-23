#ifndef TGMM_H
#define TGMM_H

#include "btracker.h"
#include "GMM/colotracker.h"


class tGMM : public BTracker
{
public:
    tGMM(float dist_adj = DIST_ADJ, float conf_adj = 1.0);

    void init(cv::Mat& image, cv::Rect region);
    void correctState(std::vector<float> st);
    void track();
    void update();
    void newFrame(cv::Mat& image, std::vector<float> predictRect);
    cv::Rect getRect();

private:
    Backgroundtracker gmm;
    cv::Mat currentFrame;
    std::vector<float> currentPredictRect;
};

#endif // TASMS_H
