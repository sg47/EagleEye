#pragma once

#include "Aquila/Nodes/Node.h"
namespace cv
{
namespace cuda
{
    void histogram(const cv::cuda::GpuMat& in, cv::cuda::GpuMat& bins, cv::cuda::GpuMat& histogram,
                   float min = 0, float max = 256,
                   cv::cuda::Stream& stream = cv::cuda::Stream::Null());
}
}
namespace aq
{
namespace Nodes
{
    class HistogramRange : public Node
    {
    public:
        MO_DERIVE(HistogramRange, Node)
            PARAM(double, lower_bound, 0.0)
            PARAM(double, upper_bound, 1.0)
            PARAM(int, bins, 100)
            INPUT(SyncedMemory, input, nullptr)
            OUTPUT(SyncedMemory, histogram, SyncedMemory())
            OUTPUT(SyncedMemory, levels, SyncedMemory())
        MO_END;
    protected:
        bool ProcessImpl();
        void updateLevels(int type);
    };
    class Histogram: public Node
    {
    public:
        MO_DERIVE(Histogram, Node)
            INPUT(SyncedMemory, input, nullptr)
            PARAM(float, min, 0)
            PARAM(float, max, 256)
            OUTPUT(SyncedMemory, histogram, {})
            OUTPUT(SyncedMemory, bins, {})
        MO_END;
    protected:
        bool ProcessImpl();
    };
}
}
