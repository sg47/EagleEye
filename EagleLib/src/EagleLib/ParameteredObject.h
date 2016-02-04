#pragma once
#include "EagleLib/Defs.hpp"
#include "IObject.h"
#include "Parameter.hpp"
#include "InputParameter.hpp"
#include "TypedParameter.hpp"
#include "TypedInputParameter.hpp"
#include <opencv2/core/persistence.hpp>
#include "type.h"
#include <boost/thread/recursive_mutex.hpp>
#include <boost/lexical_cast.hpp>
namespace EagleLib
{
    struct ParameteredObjectImpl; // Private implementation stuffs
    template<typename T> class TypedParameter;
    template<typename T> class TypedParameterPtr;
    class EAGLE_EXPORTS ParameteredObject
    {
    public:
        ParameteredObject();
        ~ParameteredObject();
        virtual void onUpdate(cv::cuda::Stream* stream);
        virtual Parameters::Parameter* addParameter(Parameters::Parameter::Ptr param);
        bool exists(const std::string& name);
        bool exists(size_t index);

        // Thows exception on unable to get parameter
        Parameters::Parameter::Ptr getParameter(int idx);
        Parameters::Parameter::Ptr getParameter(const std::string& name);

        // Returns nullptr on unable to get parameter
        Parameters::Parameter::Ptr getParameterOptional(int idx);
        Parameters::Parameter::Ptr getParameterOptional(const std::string& name);


        void RegisterParameterCallback(int idx, const boost::function<void(cv::cuda::Stream*)>& callback, bool lock_param = false, bool lock_object = false);
        void RegisterParameterCallback(const std::string& name, const boost::function<void(cv::cuda::Stream*)>& callback, bool lock_param = false, bool lock_object = false);
        void RegisterParameterCallback(Parameters::Parameter* param, const boost::function<void(cv::cuda::Stream*)>& callback, bool lock_param = false, bool lock_object = false);

        template<typename T>
        Parameters::Parameter* registerParameter(const std::string& name, T* data);

        template<typename T>
        Parameters::Parameter* addParameter(const std::string& name, const T& data);
        
        template<typename T>
        Parameters::Parameter* addIfNotExist(const std::string& name, const T& data);

        template<typename T>
        Parameters::TypedInputParameter<T>* addInputParameter(const std::string& name);

        template<typename T>
        bool updateInputQualifier(const std::string& name, const boost::function<bool(Parameters::Parameter*)>& qualifier);

        template<typename T>
        bool updateInputQualifier(int idx, const boost::function<bool(Parameters::Parameter*)>& qualifier);

        template<typename T>
        Parameters::Parameter* updateParameterPtr(const std::string& name, T* data, cv::cuda::Stream* stream = nullptr);

        template<typename T>
        Parameters::Parameter* updateParameter(const std::string& name, const T& data, cv::cuda::Stream* stream = nullptr);

        template<typename T>
        Parameters::Parameter* updateParameter(size_t idx, const T data, cv::cuda::Stream* stream = nullptr);

        template<typename T>
        typename Parameters::ITypedParameter<T>::Ptr getParameter(std::string name);

        template<typename T>
        typename Parameters::ITypedParameter<T>::Ptr getParameter(int idx);

        template<typename T>
        typename Parameters::ITypedParameter<T>::Ptr getParameterOptional(std::string name);

        template<typename T>
        typename Parameters::ITypedParameter<T>::Ptr getParameterOptional(int idx);

        std::vector<Parameters::Parameter::Ptr> parameters;
        // Mutex for blocking processing of a node during parameter update
        boost::recursive_mutex                                              mtx;
    protected:
        struct Impl;
        std::shared_ptr<ParameteredObjectImpl> _impl;

    private:
        void RunCallbackLockObject(cv::cuda::Stream* stream, const boost::function<void(cv::cuda::Stream*)>& callback);
        void RunCallbackLockParameter(cv::cuda::Stream* stream, const boost::function<void(cv::cuda::Stream*)>& callback, boost::recursive_mutex* paramMtx);
        void RunCallbackLockBoth(cv::cuda::Stream* stream, const boost::function<void(cv::cuda::Stream*)>& callback, boost::recursive_mutex* paramMtx);
    };

    class EAGLE_EXPORTS ParameteredIObject: public IObject, public ParameteredObject
    {
    public:
        virtual void Serialize(ISimpleSerializer* pSerializer);
        virtual void Init(const cv::FileNode& configNode);
        
    };


    // ****************************** Implementation ****************************************
    template<typename T>
    Parameters::Parameter* ParameteredObject::registerParameter(const std::string& name, T* data)
    {
        return addParameter(typename Parameters::TypedParameterPtr<T>::Ptr(new Parameters::TypedParameterPtr<T>(name, data)));
    }

    template<typename T>
    Parameters::Parameter* ParameteredObject::addParameter(const std::string& name, const T& data)
    {
        return addParameter(typename Parameters::TypedParameter<T>::Ptr(new Parameters::TypedParameter<T>(name, data)));
    }
    template<typename T>
    Parameters::Parameter* ParameteredObject::addIfNotExist(const std::string& name, const T& data)
    {
        if(!exists(name))
            return addParameter(typename Parameters::TypedParameter<T>::Ptr(new Parameters::TypedParameter<T>(name, data)));
        return nullptr;
    }

    template<typename T>
    Parameters::TypedInputParameter<T>* ParameteredObject::addInputParameter(const std::string& name)
    {
        auto input_param = new Parameters::TypedInputParameter<T>(name);
        addParameter(typename Parameters::TypedInputParameter<T>::Ptr(input_param));
        return input_param;
    }

    template<typename T>
    bool ParameteredObject::updateInputQualifier(const std::string& name, const boost::function<bool(Parameters::Parameter*)>& qualifier_)
    {
        auto param = getParameter(name);
        if (param && param->type & Parameters::Parameter::Input)
        {
            Parameters::InputParameter* inputParam = dynamic_cast<Parameters::InputParameter*>(param.get());
            if (inputParam)
            {
                inputParam->SetQualifier(qualifier_);
                return true;
            }
        }
        return false;
    }

    template<typename T>
    bool ParameteredObject::updateInputQualifier(int idx, const boost::function<bool(Parameters::Parameter*)>& qualifier)
    {
        auto param = getParameter<T>(idx);
        if (param && param->type & Parameters::Parameter::Input)
        {
            Parameters::InputParameter* inputParam = dynamic_cast<Parameters::InputParameter*>(param.get());
            if (inputParam)
            {
                inputParam->SetQualifier(qualifier);
                return true;
            }
        }
        return false;
    }

    template<typename T>
    Parameters::Parameter* ParameteredObject::updateParameterPtr(const std::string& name, T* data, cv::cuda::Stream* stream)
    {
        typename Parameters::ITypedParameter<T>::Ptr param;
        param = getParameterOptional<T>(name);
        if (param == nullptr)
        {
            BOOST_LOG_TRIVIAL(debug) << name << " doesn't exist, adding";
            return registerParameter<T>(name, data);
        }
        if(auto non_ref_param = std::dynamic_pointer_cast<Parameters::TypedParameter<T>>(param))
        {
            // Parameter exists but is of the wrong type, due to being loaded from a file as a generic typed parameter
            *data = *non_ref_param->Data();
            // Find the incorrectly typed parameter
            for(int i = 0; i < parameters.size(); ++i)
            {
                if(parameters[i]->GetName() == name)
                {
                    parameters[i] = typename Parameters::TypedParameterPtr<T>::Ptr(new Parameters::TypedParameterPtr<T>(name, data));
                }
            }
        }
        param->UpdateData(data);
        onUpdate(stream);
        return param.get();
    }

    template<typename T> 
    Parameters::Parameter* ParameteredObject::updateParameter(const std::string& name, const T& data, cv::cuda::Stream* stream)
    {
        typename Parameters::ITypedParameter<T>::Ptr param;
        param = getParameterOptional<T>(name);
        if (param == nullptr)
        {
            BOOST_LOG_TRIVIAL(debug) << "Parameter named \"" << name << "\" with type " << Loki::TypeInfo(typeid(T)).name() << " doesn't exist, adding";
            return addParameter<T>(name, data);
        }
        param->UpdateData(data, stream);
        onUpdate(stream);
        return param.get();
    }

    template<typename T>
    Parameters::Parameter* ParameteredObject::updateParameter(size_t idx, const T data, cv::cuda::Stream* stream)
    {
        if (idx > parameters.size() || idx < 0)
            return nullptr;
        auto param = std::dynamic_pointer_cast<Parameters::ITypedParameter<T>>(parameters[idx]);
        if (param == NULL)
            return nullptr;
        param->UpdateData(data, stream);
        onUpdate(stream);
        return param.get();
    }

    template<typename T>
    typename Parameters::ITypedParameter<T>::Ptr ParameteredObject::getParameter(std::string name)
    {
        auto param = getParameter(name);
        if (param == nullptr)
        {
            throw cv::Exception(0, "Failed to get parameter by name " + name, __FUNCTION__, __FILE__, __LINE__);
            return typename Parameters::ITypedParameter<T>::Ptr();
        }
        auto typedParam = std::dynamic_pointer_cast<typename Parameters::ITypedParameter<T>>(param);
        if (typedParam == nullptr)
            throw cv::Exception(0, "Failed to cast parameter to the appropriate type, requested type: " +
                TypeInfo::demangle(typeid(T).name()) + " parameter actual type: " + param->GetTypeInfo().name(), __FUNCTION__, __FILE__, __LINE__);

        return typedParam;
    }

    template<typename T>
    typename Parameters::ITypedParameter<T>::Ptr ParameteredObject::getParameter(int idx)
    {
        auto param = getParameter(idx);
        if (param == nullptr)
            throw cv::Exception(0, "Failed to get parameter by index " + boost::lexical_cast<std::string>(idx), __FUNCTION__, __FILE__, __LINE__);

        auto typedParam = std::dynamic_pointer_cast<typename Parameters::ITypedParameter<T>>(param);
        if (typedParam == nullptr)
            throw cv::Exception(0, "Failed to cast parameter to the appropriate type, requested type: " +
                TypeInfo::demangle(typeid(T).name()) + " parameter actual type: " + param->GetTypeInfo().name(), __FUNCTION__, __FILE__, __LINE__);
        return typedParam;
    }


    template<typename T>
    typename Parameters::ITypedParameter<T>::Ptr ParameteredObject::getParameterOptional(std::string name)
    {
        auto param = getParameterOptional(name);
        if (param == nullptr)
        {
            return typename Parameters::ITypedParameter<T>::Ptr();
        }
        auto typedParam = std::dynamic_pointer_cast<typename Parameters::ITypedParameter<T>>(param);
        if (typedParam == nullptr)
            BOOST_LOG_TRIVIAL(debug) << "Failed to cast parameter to the appropriate type, requested type: " <<
                TypeInfo::demangle(typeid(T).name()) << " parameter actual type: " << param->GetTypeInfo().name();

        return typedParam;
    }

    template<typename T>
    typename Parameters::ITypedParameter<T>::Ptr ParameteredObject::getParameterOptional(int idx)
    {
        auto param = getParameterOptional(idx);
        if (param == nullptr)
            return typename Parameters::ITypedParameter<T>::Ptr(); // Return a nullptr

        return std::dynamic_pointer_cast<typename Parameters::ITypedParameter<T>>(param);
    }
}