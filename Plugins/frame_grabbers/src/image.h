#pragma once

#include "EagleLib/Nodes/IFrameGrabber.hpp"
#include "EagleLib/ICoordinateManager.h"

namespace EagleLib
{
    namespace Nodes
    {
    
    class PLUGIN_EXPORTS frame_grabber_image: public IFrameGrabber
    {
    public:
        MO_DERIVE(frame_grabber_image, IFrameGrabber);
        MO_END;


        virtual bool LoadFile(const ::std::string& file_path);
        virtual long long GetFrameNumber();
        virtual long long GetNumFrames();
        virtual ::std::string GetSourceFilename();

        virtual TS<SyncedMemory> GetCurrentFrame(cv::cuda::Stream& stream);
        virtual TS<SyncedMemory> GetFrame(int index, cv::cuda::Stream& stream);
        virtual TS<SyncedMemory> GetNextFrame(cv::cuda::Stream& stream);
        virtual TS<SyncedMemory> GetFrameRelative(int index, cv::cuda::Stream& stream);

        virtual rcc::shared_ptr<ICoordinateManager> GetCoordinateManager();
        static int CanLoadDocument(const ::std::string& document);
    private:
        cv::cuda::GpuMat                d_image;
        cv::Mat                         h_image;
        rcc::shared_ptr<ICoordinateManager>  coordinate_manager;
        ::std::string                     loaded_file;
    };
    }
}