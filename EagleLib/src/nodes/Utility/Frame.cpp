#include "nodes/Utility/Frame.h"

using namespace EagleLib;
void FrameRate::Init(bool firstInit)
{

}

cv::cuda::GpuMat FrameRate::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
    boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration delta = currentTime - prevTime;
    prevTime = currentTime;
    updateParameter<double>("Framerate", 1000.0 / delta.total_milliseconds(), Parameter::State);
    return img;
}

void FrameLimiter::Init(bool firstInit)
{

}

cv::cuda::GpuMat FrameLimiter::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
	return img;
}
void CreateMat::Init(bool firstInit)
{
    if(firstInit)
    {
        EnumParameter dataType;
        dataType.addEnum(ENUM(CV_8U));
        dataType.addEnum(ENUM(CV_8S));
        dataType.addEnum(ENUM(CV_16U));
        dataType.addEnum(ENUM(CV_16S));
        dataType.addEnum(ENUM(CV_32S));
        dataType.addEnum(ENUM(CV_32F));
        dataType.addEnum(ENUM(CV_64F));
        updateParameter("Datatype", dataType);
        updateParameter("Channels", int(1));
        updateParameter("Width", 1920);
        updateParameter("Height", 10280);
        updateParameter("Fill", cv::Scalar::all(0));
    }
}

cv::cuda::GpuMat CreateMat::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
    if(parameters[0]->changed ||
       parameters[1]->changed ||
       parameters[2]->changed ||
       parameters[3]->changed ||
       parameters[4]->changed )
    {
        TypedParameter<EnumParameter>::Ptr typeParam = getParameter<EnumParameter>(0);
        int selection = typeParam->data.currentSelection;
        int dtype = typeParam->data.values[selection];
        createdMat = cv::cuda::GpuMat(getParameter<int>(3)->data,
                    getParameter<int>(2)->data,
                    dtype, getParameter<cv::Scalar>(4)->data);
        updateParameter("Output", createdMat, Parameter::Output);
    }
    return img;
}
void SetMatrixValues::Init(bool firstInit)
{
    if(firstInit)
    {
        addInputParameter<cv::cuda::GpuMat>("Input image");
        addInputParameter<cv::cuda::GpuMat>("Input mask");
        updateParameter("Replace value", cv::Scalar(0,0,0));
    }
}

cv::cuda::GpuMat SetMatrixValues::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
    cv::cuda::GpuMat* input = getParameter<cv::cuda::GpuMat*>(0)->data;
    if(input == nullptr)
        input = &img;
    cv::cuda::GpuMat* mask = getParameter<cv::cuda::GpuMat*>(1)->data;
    if(mask && mask->size() == input->size())
    {
        input->setTo(getParameter<cv::Scalar>(2)->data, *mask, stream);
    }else
    {
        input->setTo(getParameter<cv::Scalar>(2)->data, stream);
    }
    return *input;
}

NODE_DEFAULT_CONSTRUCTOR_IMPL(SetMatrixValues)
NODE_DEFAULT_CONSTRUCTOR_IMPL(FrameRate)
NODE_DEFAULT_CONSTRUCTOR_IMPL(FrameLimiter)
NODE_DEFAULT_CONSTRUCTOR_IMPL(CreateMat)
