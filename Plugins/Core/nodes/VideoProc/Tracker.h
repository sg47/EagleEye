#pragma once

#include <EagleLib/nodes/Node.h>
#include <EagleLib/nodes/VideoProc/Tracking.hpp>
#include <boost/circular_buffer.hpp>
#include "EagleLib/utilities/CudaUtils.hpp"
#include "RuntimeInclude.h"
#include "RuntimeSourceDependency.h"
RUNTIME_COMPILER_SOURCEDEPENDENCY
RUNTIME_MODIFIABLE_INCLUDE
namespace EagleLib
{
    namespace Nodes
    {
    
    class KeyFrameTracker: public Node
    {
        // Used to find homography once the data has been downloaded from the stream
        ConstBuffer<TrackingResults*> homographyBuffer;
        ConstBuffer<cv::cuda::GpuMat> warpedImageBuffer;
        ConstBuffer<cv::cuda::GpuMat> warpedMaskBuffer;
        ConstBuffer<cv::cuda::GpuMat> d_displayBuffer;
        cv::cuda::GpuMat nonWarpedMask;
        boost::circular_buffer<TrackedFrame> trackedFrames;
        std::map<int, KeyFrame> keyFrames;

    public:
        KeyFrameTracker();
        virtual void NodeInit(bool firstInit);
        virtual cv::cuda::GpuMat doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream = cv::cuda::Stream::Null());
        virtual void reset();
    };

    class CMTTracker: public Node
    {

    public:
        CMTTracker();
        virtual void NodeInit(bool firstInit);
        virtual cv::cuda::GpuMat doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream = cv::cuda::Stream::Null());

    };

    class TLDTracker:public Node
    {
    public:
        TLDTracker();
        virtual void NodeInit(bool firstInit);
        virtual cv::cuda::GpuMat doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream = cv::cuda::Stream::Null());
    };
    }
}
