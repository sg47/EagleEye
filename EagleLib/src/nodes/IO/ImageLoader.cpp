#include "nodes/IO/ImageLoader.h"
#include <opencv2/imgcodecs.hpp>
using namespace EagleLib;
RUNTIME_COMPILER_LINKLIBRARY("-lopencv_imgcodecs")


void ImageLoader::Init(bool firstInit)
{
    Node::Init(firstInit);
    updateParameter<boost::filesystem::path>("Filename", boost::filesystem::path("/home/dmoodie/Downloads/oimg.jpeg"), Parameter::Control, "Path to image file");
    parameters[0]->changed = true;
}
void ImageLoader::load()
{
    auto path = getParameter<boost::filesystem::path>(0);
    if(path)
    {
        if(boost::filesystem::exists(path->data))
        {
            cv::Mat h_img = cv::imread(path->data.string()); 
            d_img.upload(h_img); 
        }else
        {
            log(Status, "File doesn't exist");
        }
    }
}

cv::cuda::GpuMat ImageLoader::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
    TIME
    if(parameters[0]->changed)
    {
        load();
        parameters[0]->changed = false;
    }
    TIME
	if (!d_img.empty())
    {
        TIME
        d_img.copyTo(img,stream);
        TIME
    }
    return img;
}

bool ImageLoader::SkipEmpty() const
{
    return false;
}
bool DirectoryLoader::SkipEmpty() const
{
    return false;
}
void DirectoryLoader::Init(bool firstInit)
{
    updateParameter<ReadDirectory>("Directory", boost::filesystem::path("/home/dan/build/EagleEye/bin"), Parameter::Control, "Path to directory");
    updateParameter<bool>("Repeat", true);
    updateParameter<boost::function<void(void)>>("Restart", boost::bind(&DirectoryLoader::restart, this));

    fileIdx = 0;
}

void DirectoryLoader::restart()
{
    fileIdx = 0;
}

cv::cuda::GpuMat DirectoryLoader::doProcess(cv::cuda::GpuMat &img, cv::cuda::Stream& stream)
{
    if(parameters[0]->changed)
    {
        boost::filesystem::path& path = getParameter<ReadDirectory>(0)->data;
        files.clear();
        if(boost::filesystem::is_directory(path))
        {
            for(boost::filesystem::directory_iterator itr(path); itr != boost::filesystem::directory_iterator(); ++itr)
            {
                if(boost::filesystem::is_regular_file(itr->status()))
                    files.push_back(itr->path().string());
            }
        }
    }
    if(files.size())
    {
        if(fileIdx < files.size())
        {
            cv::Mat h_img = cv::imread(files[fileIdx]);
            updateParameter("Current file", files[fileIdx]);
            img.upload(h_img, stream);
            ++fileIdx;
        }
        if(getParameter<bool>(1)->data)
            if(fileIdx == files.size())
                fileIdx = 0;
    }
    return img;
}


NODE_DEFAULT_CONSTRUCTOR_IMPL(ImageLoader)
NODE_DEFAULT_CONSTRUCTOR_IMPL(DirectoryLoader)
