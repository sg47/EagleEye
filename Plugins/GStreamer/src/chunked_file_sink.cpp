#include "chunked_file_sink.h"
#include <EagleLib/Nodes/FrameGrabberInfo.hpp>
#include <EagleLib/ICoordinateManager.h>
#include <gst/base/gstbasesink.h>
using namespace EagleLib;


int chunked_file_sink::CanLoadDocument(const std::string& document)
{
    return 0; // Currently needs to be manually specified
}
int chunked_file_sink::LoadTimeout()
{
    return 3000;
}



GstFlowReturn chunked_file_sink::on_pull()
{
    GstSample *sample = gst_base_sink_get_last_sample(GST_BASE_SINK(_appsink));
    //g_signal_emit_by_name(_appsink, "pull-sample", &sample, NULL);
    if(sample)
    {
        GstBuffer *buffer;
        GstCaps *caps;
        GstStructure *s;
        GstMapInfo map;
        caps = gst_sample_get_caps (sample);
        if (!caps) 
        {
            LOG(debug) << "could not get sample caps";
            return GST_FLOW_OK;
        }
        s = gst_caps_get_structure (caps, 0);
        gint width, height;
        gboolean res; 
        res = gst_structure_get_int (s, "width", &width);
        res |= gst_structure_get_int (s, "height", &height);
        const gchar* format = gst_structure_get_string(s, "format");
        if (!res) 
        {
            LOG(debug) << "could not get snapshot dimension\n";
            return GST_FLOW_OK;
        }
        buffer = gst_sample_get_buffer (sample);
        if (gst_buffer_map (buffer, &map, GST_MAP_READ))
        {
            cv::Mat mapped(height, width, CV_8UC3);
            memcpy(mapped.data, map.data, map.size);

        }
        gst_sample_unref (sample);
    }
    return GST_FLOW_OK;
}
bool chunked_file_sink::LoadFile(const std::string& file_path)
{
    if(gstreamer_src_base::create_pipeline(file_path))
    {
        _filesink = gst_bin_get_by_name(GST_BIN(_pipeline), "filesink0");
        if(_filesink)
        {
            this->start_pipeline();
            return true;
        }
    }
    return false;
}
TS<SyncedMemory> chunked_file_sink::GetCurrentFrame(cv::cuda::Stream& stream)
{
    return TS<SyncedMemory>();
}
long long chunked_file_sink::GetNumFrames()
{
    return -1;
}

rcc::shared_ptr<EagleLib::ICoordinateManager> chunked_file_sink::GetCoordinateManager()
{
    return rcc::shared_ptr<EagleLib::ICoordinateManager>();
}

bool chunked_file_sink::ProcessImpl()
{
    return false;
}

MO_REGISTER_CLASS(chunked_file_sink);