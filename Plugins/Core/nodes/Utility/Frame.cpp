#include "nodes/Utility/Frame.h"
#include "external_includes/cv_cudawarping.hpp"
#include "external_includes/cv_cudaarithm.hpp"
#include <Qualifiers.hpp>

using namespace EagleLib;
void FrameRate::Init(bool firstInit)
{

}

cv::cuda::GpuMat FrameRate::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
    boost::posix_time::ptime currentTime = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration delta = currentTime - prevTime;
    prevTime = currentTime;
    updateParameter<double>("Framerate", 1000.0/delta.total_milliseconds(), Parameters::Parameter::State);
    return img;
}

void FrameLimiter::Init(bool firstInit)
{
	updateParameter<double>("Framerate", 60.0);
}

cv::cuda::GpuMat FrameLimiter::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
	auto currentTime = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration delta(currentTime - lastTime);
	lastTime = currentTime;
	int goalTime = 1000.0 / *getParameter<double>(0)->Data();
	if (delta.total_milliseconds() < goalTime)
	{
		//boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
		boost::this_thread::sleep_for(boost::chrono::milliseconds(goalTime - delta.total_milliseconds()));
	}
	return img;
}
void CreateMat::Init(bool firstInit)
{
    if(firstInit)
    {
		Parameters::EnumParameter dataType;
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
        updateParameter("Height", 1080);
        updateParameter("Fill", cv::Scalar::all(0));
    }
    parameters[0]->changed = true;
}

cv::cuda::GpuMat CreateMat::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
    if(parameters[0]->changed ||
       parameters[1]->changed ||
       parameters[2]->changed ||
       parameters[3]->changed ||
       parameters[4]->changed )
    {
        auto typeParam = getParameter<Parameters::EnumParameter>(0);
		int selection = typeParam->Data()->currentSelection;
		int dtype = typeParam->Data()->values[selection];
		createdMat = cv::cuda::GpuMat(*getParameter<int>(3)->Data(),
					*getParameter<int>(2)->Data(),
					dtype, *getParameter<cv::Scalar>(4)->Data());
        updateParameter("Output", createdMat, Parameters::Parameter::Output);
        parameters[0]->changed = false;
        parameters[1]->changed = false;
        parameters[2]->changed = false;
        parameters[3]->changed = false;
        parameters[4]->changed = false;
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
    qualifiersSetup = false;
}

cv::cuda::GpuMat SetMatrixValues::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
    TIME
		cv::cuda::GpuMat* input = getParameter<cv::cuda::GpuMat>(0)->Data();
    if(input == nullptr)
        input = &img;
    TIME
    if(parameters[0]->changed || qualifiersSetup == false)
    {
        boost::function<bool(Parameters::Parameter*)> f = GpuMatQualifier::get(input->cols, input->rows, 1, CV_8U);
        updateInputQualifier<cv::cuda::GpuMat>(1, f);
    }
	cv::cuda::GpuMat* mask = getParameter<cv::cuda::GpuMat>(1)->Data();

    if(mask && mask->size() == input->size())
    {
        TIME
			input->setTo(*getParameter<cv::Scalar>(2)->Data(), *mask, stream);
        TIME
    }else
    {
        TIME
        input->setTo(*getParameter<cv::Scalar>(2)->Data(), stream);
        TIME
    }
    return *input;
}
void Resize::Init(bool firstInit)
{
	updateParameter("Width", int(224));
	updateParameter("Height", int(224));
}
cv::cuda::GpuMat Resize::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
	auto buf = bufferPool.getFront();
	cv::cuda::resize(img, buf->data, cv::Size(*getParameter<int>(0)->Data(), *getParameter<int>(1)->Data()), 0.0, 0.0, 1, stream);
	return buf->data;
}
void Subtract::Init(bool firstInit)
{
	updateParameter("Value to subtract", cv::Scalar(0, 0, 0));
}
cv::cuda::GpuMat Subtract::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
	cv::cuda::subtract(img, *getParameter<cv::Scalar>(0)->Data(), img, cv::noArray(), -1, stream);
	return img;
}
NODE_DEFAULT_CONSTRUCTOR_IMPL(SetMatrixValues)
NODE_DEFAULT_CONSTRUCTOR_IMPL(FrameRate)
NODE_DEFAULT_CONSTRUCTOR_IMPL(FrameLimiter)
NODE_DEFAULT_CONSTRUCTOR_IMPL(CreateMat)
NODE_DEFAULT_CONSTRUCTOR_IMPL(Resize)
NODE_DEFAULT_CONSTRUCTOR_IMPL(Subtract)