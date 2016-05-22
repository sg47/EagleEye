#pragma once

#include <EagleLib/nodes/Node.h>
#include "EagleLib/utilities/CudaUtils.hpp"
#include "RuntimeInclude.h"
#include "RuntimeSourceDependency.h"
RUNTIME_COMPILER_SOURCEDEPENDENCY
RUNTIME_MODIFIABLE_INCLUDE
namespace EagleLib
{
    namespace Nodes
    {
    
    class FFT: public Node
    {
        enum output
        {
            Coefficients = -1,
            Magnitude = 0,
            Phase = 1
        };
        ConstBuffer<cv::cuda::GpuMat> destBuf;
        ConstBuffer<cv::cuda::GpuMat> floatBuf;
    public:
        FFT();
        virtual void NodeInit(bool firstInit);
        virtual cv::cuda::GpuMat doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream = cv::cuda::Stream::Null());
    };

    class FFTPreShiftImage: public Node
    {
        cv::cuda::GpuMat d_shiftMat;
    public:
        FFTPreShiftImage();
        virtual void NodeInit(bool firstInit);
        virtual cv::cuda::GpuMat doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream = cv::cuda::Stream::Null());
    };

    class FFTPostShift: public Node
    {
        cv::cuda::GpuMat d_shiftMat;
    public:
        FFTPostShift();
        virtual void NodeInit(bool firstInit);
        virtual cv::cuda::GpuMat doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream = cv::cuda::Stream::Null());
    };
    }
}
