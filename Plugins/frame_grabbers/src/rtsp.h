#pragma once

#include "gstreamer.h"
#include "EagleLib/ICoordinateManager.h"

namespace EagleLib
{
    class PLUGIN_EXPORTS frame_grabber_rtsp: public frame_grabber_gstreamer
    {
    public:
        class PLUGIN_EXPORTS frame_grabber_rtsp_info: public FrameGrabberInfo
        {
        public:
            virtual std::string GetObjectName();
            virtual std::string GetObjectTooltip();
            virtual std::string GetObjectHelp();
            virtual int CanLoadDocument(const std::string& document) const;
            virtual int Priority() const;
            virtual int LoadTimeout() const;
        };
        frame_grabber_rtsp();
        virtual void Init(bool firstInit);
        virtual bool LoadFile(const std::string& file_path = "");
        virtual TS<SyncedMemory> GetNextFrameImpl(cv::cuda::Stream& stream);

        virtual rcc::shared_ptr<ICoordinateManager> GetCoordinateManager();
        
    protected:
        rcc::shared_ptr<ICoordinateManager>          coordinate_manager;
        size_t frame_count;
        bool _reconnect;
    };
}