#pragma once
#include "MetaObject/MetaObject.hpp"
#include "gstreamer.hpp"
#include "EagleLib/Nodes/IFrameGrabber.hpp"

namespace EagleLib
{
    namespace Nodes
    {
        class PLUGIN_EXPORTS tcpserver: public gstreamer_sink_base
        {
            bool _initialized;
        public:
            enum {
                None = -1
            };
            MO_DERIVE(tcpserver, gstreamer_sink_base)
                ENUM_PARAM(encoders, None);
                ENUM_PARAM(interfaces, None);
                INPUT(SyncedMemory, image, nullptr);
            MO_END;
            tcpserver();
            ~tcpserver();
            virtual void NodeInit(bool firstInit);
            bool ProcessImpl();
            
        };
    }

    class PLUGIN_EXPORTS BufferedHeartbeatRtsp: public Nodes::FrameGrabberBuffered, public gstreamer_src_base
    {
    public:
        virtual void NodeInit(bool firstInit);

    protected:
        
    };


}