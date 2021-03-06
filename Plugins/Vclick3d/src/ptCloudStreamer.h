#pragma once

#include "Aquila/Nodes/Node.h"
#include <QtNetwork/qtcpserver.h>
#include <QtNetwork/qtcpsocket.h>
#include "RuntimeInclude.h"
#include "RuntimeSourceDependency.h"
/*
RUNTIME_COMPILER_SOURCEDEPENDENCY_FILE(PROJECT_BUILD_DIR "/include/moc_ptCloudStreamer", ".cpp");

namespace aq
{
    namespace Nodes
    {
    
    class ServerHandler: public QObject
    {
        Q_OBJECT;
        QTcpServer* server;
        std::vector<QTcpSocket*> connections;
    public:
        ServerHandler();
        void send();

    public slots :
        void on_newConnection();
        

    };
    class Server : public Node
    {
        std::shared_ptr<ServerHandler> handler;
    public:
        Server();

        virtual void NodeInit(bool firstInit);
        virtual cv::cuda::GpuMat doProcess(cv::cuda::GpuMat& img, cv::cuda::Stream& stream);
    };
    }
}
*/