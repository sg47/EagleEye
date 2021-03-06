

#include "Aquila/Nodes/Node.h"
//#include <opencv2/highgui.hpp>
//RUNTIME_COMPILER_LINKLIBRARY("-lopencv_core -lopencv_highgui")
#define PARAMTESTMACRO(type) updateParameter< type >(##type,0)
#if 0
namespace aq
{
    class TestNode: public Node
    {
    public:
        TestNode():Node()
        {
            nodeName = "TestNode";
            treeName = nodeName;
            //parent = nullptr;
        }

        ~TestNode(){}
        void NodeInit(bool firstInit)
        {
            updateParameter<int>("int",0);
            updateParameter<short>("short",0);
            //updateParameter<char>("char",0);
            updateParameter<unsigned int>("unsigned int",0);
            updateParameter<unsigned short>("unsigned short",0);
            updateParameter<unsigned char>("unsigned char",0);

            updateParameter<int*>("int*",0);
            updateParameter<short*>("short*",0);
            //updateParameter<char*>("char*",0);
            updateParameter<unsigned int*>("unsigned int*",0);
            updateParameter<unsigned short*>("unsigned short*",0);
            updateParameter<unsigned char*>("unsigned char*",0);

            updateParameter<float>("float",0);
            updateParameter<double>("double",0);
            updateParameter<float*>("float*",0);
            updateParameter<double*>("double*",0);

            updateParameter<std::string>("std::string","test string");
            updateParameter<std::string*>("std::string*",nullptr);


            updateParameter<boost::function<void(void)>>("Function", boost::bind(&TestNode::testFunction,this));

            updateParameter<cv::Scalar>("cv::Scalar", cv::Scalar());
            updateParameter<std::vector<int>>("std::vector<int>", std::vector<int>());
            getParameter<std::vector<int>>("std::vector<int>")->Data()->push_back(10);
            getParameter<std::vector<int>>("std::vector<int>")->Data()->push_back(15);
            updateParameter<std::vector<double>>("std::vector<double>", std::vector<double>());
            getParameter<std::vector<double>>("std::vector<double>")->Data()->push_back(10.5);
            getParameter<std::vector<double>>("std::vector<double>")->Data()->push_back(15.5);

            /*vec_ID vecID;
            vecID.push_back(std::pair<int,double>(0, 0.5));
            vecID.push_back(std::pair<int,double>(1, 0.6));
            vecID.push_back(std::pair<int,double>(2, 0.7));
            updateParameter<vec_ID>("std::vector<std::pair<int,double>> control", vecID);
            updateParameter<vec_ID>("std::vector<std::pair<int,double>> state", vecID, mo::IParameter::State);
            vec_DD vecDD;
            vecID.push_back(std::pair<double,double>(0, 0.5));
            vecID.push_back(std::pair<double,double>(1, 0.6));
            vecID.push_back(std::pair<double,double>(2, 0.7));
            updateParameter<vec_DD>("std::vector<std::pair<double,double>> control", vecDD);
            updateParameter<vec_DD>("std::vector<std::pair<double,double>> state", vecDD, mo::IParameter::State);

            vec_II vecII;
            vecII.push_back(std::pair<int,int>(0, 3));
            vecII.push_back(std::pair<int,int>(1, 2));
            vecII.push_back(std::pair<int,int>(2, 1));
            updateParameter<vec_II>("std::vector<std::pair<int,int>> control", vecII);
            updateParameter<vec_II>("std::vector<std::pair<int,int>> state", vecII, mo::IParameter::State);

            vec_DI vecDI;
            vecDI.push_back(std::pair<double, int>(0.5, 3));
            vecDI.push_back(std::pair<double, int>(1.2, 2));
            vecDI.push_back(std::pair<double, int>(2.3, 1));
            updateParameter<vec_DI>("std::vector<std::pair<double,int>> control", vecDI, mo::IParameter::State);
            updateParameter<vec_DI>("std::vector<std::pair<double,int>> state", vecDI, mo::IParameter::State);*/

            updateParameter<int>("int state", 0, mo::IParameter::State);
            updateParameter<short>("short state", 0, mo::IParameter::State);
            //updateParameter<char>("char state", 0, mo::IParameter::State);
            updateParameter<unsigned int>("unsigned int state", 0, mo::IParameter::State);
            updateParameter<unsigned short>("unsigned short state", 0, mo::IParameter::State);
            updateParameter<unsigned char>("unsigned char state", 0, mo::IParameter::State);

            updateParameter<int*>("int* state", 0, mo::IParameter::State);
            updateParameter<short*>("short* state", 0, mo::IParameter::State);
            //updateParameter<char*>("char* state", 0, mo::IParameter::State);
            updateParameter<unsigned int*>("unsigned int* state", 0, mo::IParameter::State);
            updateParameter<unsigned short*>("unsigned short* state", 0, mo::IParameter::State);
            //updateParameter<unsigned char*>("unsigned char* state", 0, mo::IParameter::State);

            updateParameter<float>("float state", 0, mo::IParameter::State);
            updateParameter<double>("double state", 0, mo::IParameter::State);
            updateParameter<float*>("float* state", 0, mo::IParameter::State);
            updateParameter<double*>("double* state", 0, mo::IParameter::State);

            updateParameter<std::string>("std::string state", "test string", mo::IParameter::State);
            updateParameter<std::string*>("std::string* state", nullptr, mo::IParameter::State);

            updateParameter<cv::Mat>("cv::Mat state", cv::Mat(), mo::IParameter::State);
            updateParameter<cv::Scalar>("cv::Scalar state", cv::Scalar(), mo::IParameter::State);
            updateParameter<std::vector<int>>("std::vector<int> state", std::vector<int>(), mo::IParameter::State);
            getParameter<std::vector<int>>("std::vector<int> state")->Data()->push_back(11);
            getParameter<std::vector<int>>("std::vector<int> state")->Data()->push_back(12);
            updateParameter<std::vector<double>>("std::vector<double> state", std::vector<double>(), mo::IParameter::State);
            getParameter<std::vector<double>>("std::vector<double> state")->Data()->push_back(11.5);
            getParameter<std::vector<double>>("std::vector<double> state")->Data()->push_back(12.5);
            updateParameter<int>("int output", 0, mo::IParameter::Output);
            updateParameter<short>("short output", 0, mo::IParameter::Output);
            //updateParameter<char>("char output", 0, mo::IParameter::Output);
            updateParameter<unsigned int>("unsigned int output", 0, mo::IParameter::Output);
            updateParameter<unsigned short>("unsigned short output", 0, mo::IParameter::Output);
            //updateParameter<unsigned char>("unsigned char output", 0, mo::IParameter::Output);

            updateParameter<int*>("int* output", 0, mo::IParameter::Output);
            updateParameter<short*>("short* output", 0, mo::IParameter::Output);
            //updateParameter<char*>("char* output", 0, mo::IParameter::Output);
            updateParameter<unsigned int*>("unsigned int* output", 0, mo::IParameter::Output);
            updateParameter<unsigned short*>("unsigned short* output", 0, mo::IParameter::Output);
            //updateParameter<unsigned char*>("unsigned char* output", 0, mo::IParameter::Output);

            updateParameter<float>("float output", 0, mo::IParameter::Output);
            updateParameter<double>("double output", 0, mo::IParameter::Output);
            updateParameter<float*>("float* output", 0, mo::IParameter::Output);
            updateParameter<double*>("double* output", 0, mo::IParameter::Output);

            updateParameter<std::string>("std::string output", "test string", mo::IParameter::Output);
            updateParameter<std::string*>("std::string* output", nullptr, mo::IParameter::Output);

            updateParameter<cv::Mat>("cv::Mat output", cv::Mat(), mo::IParameter::Output);

            updateParameter<int>("int input", 0, mo::IParameter::Input);
            updateParameter<short>("short input", 0, mo::IParameter::Input);
            //updateParameter<char>("char input", 0, mo::IParameter::Input);
            updateParameter<unsigned int>("unsigned int input", 0, mo::IParameter::Input);
            updateParameter<unsigned short>("unsigned short input", 0, mo::IParameter::Input);
            //updateParameter<unsigned char>("unsigned char input", 0, mo::IParameter::Input);

            updateParameter<int*>("int* input", 0, mo::IParameter::Input);
            updateParameter<short*>("short* input", 0, mo::IParameter::Input);
            //updateParameter<char*>("char* input", 0, mo::IParameter::Input);
            updateParameter<unsigned int*>("unsigned int* input", 0, mo::IParameter::Input);
            updateParameter<unsigned short*>("unsigned short* input", 0, mo::IParameter::Input);
            //updateParameter<unsigned char*>("unsigned char* input", 0, mo::IParameter::Input);

            updateParameter<float>("float input", 0, mo::IParameter::Input);
            updateParameter<double>("double input", 0, mo::IParameter::Input);
            updateParameter<float*>("float* input", 0, mo::IParameter::Input);
            updateParameter<double*>("double* input", 0, mo::IParameter::Input);

            updateParameter<std::string>("std::string input", "test string", mo::IParameter::Input);
            updateParameter<std::string*>("std::string* input", nullptr, mo::IParameter::Input);
        }
        

        cv::cuda::GpuMat doProcess(cv::cuda::GpuMat &img)
        {
            std::cout << "test" << std::endl;
            //cv::imshow("Test", cv::Mat(img));
            return img;
        }
        void testFunction()
        {
            //log(Status, "Test function called");
            NODE_LOG(info) << "Test function called";
        }
        //boost::function<void(void)> function;

    };


}
using namespace aq;
REGISTERCLASS(TestNode)
#endif

