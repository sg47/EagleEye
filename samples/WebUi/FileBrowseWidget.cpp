#include "FileBrowseWidget.hpp"
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WSuggestionPopup>
#include <Wt/WSortFilterProxyModel>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WSignal>
using namespace Wt;

AutoCompleteFileWidget::AutoCompleteFileWidget(WContainerWidget* parent, const std::string& upload_dir,
                                   const boost::filesystem::path & current_path):
    WContainerWidget(parent),
    _current_path(current_path),
    _sig_file_selected("fileSelected", this)
{
    _txt_manual_entry = new WLineEdit(this);

    WSuggestionPopup::Options options;
    options.highlightBeginTag = "<span class=\"highlight\">";
    options.highlightEndTag = "</span>";
    //options.listSeparator = ',';
    options.whitespace = " \\n";
    options.wordSeparators = "-., \"@\\n;";

    _sp = new WSuggestionPopup(options, this);
    _sp->filterModel().connect(this, &FileBrowseWidget::onFilterModel);

    boost::filesystem::directory_iterator end_itr;
    //_current_path = g_ctx._current_dir;
    if (boost::filesystem::is_directory(current_path))
    {
        for (boost::filesystem::directory_iterator itr(current_path); itr != end_itr; ++itr)
        {
            if (boost::filesystem::is_regular_file(itr->path()))
            {
                std::string file = itr->path().stem().string();

                _sp->addSuggestion(file);
            }
        }
    }
#ifdef _MSC_VER

#else
    for(boost::filesystem::directory_iterator itr("/"); itr != end_itr; ++itr)
    {
        _sp->addSuggestion(itr->path().string());
    }
#endif
    _sp->forEdit(_txt_manual_entry);
    _sp->setFilterLength(-1);


    _txt_manual_entry->enterPressed().connect(std::bind([this]()
    {
        auto txt = _txt_manual_entry->text();
        if(!txt.empty())
        {
            //_sig_file_selected(txt.toUTF8(), "");
            _sig_file_selected.emit();
        }
    }));
}

EventSignal<>& AutoCompleteFileWidget::fileSelected()
{
    return _sig_file_selected;
}

void AutoCompleteFileWidget::onFilterModel(const WString& data_)
{
    boost::filesystem::directory_iterator end_itr;
    std::string data = data_.toUTF8();
    for (boost::filesystem::directory_iterator itr(_current_path); itr != end_itr; ++itr)
    {
        if(data == itr->path().string())
        {
            if(boost::filesystem::is_directory(itr->path()))
            {
                _sp->clearSuggestions();
                for (boost::filesystem::directory_iterator itr2(itr->path()); itr2 != end_itr; ++itr2)
                {
                    _sp->addSuggestion(itr2->path().string());
                }
                return;
            }
        }
    }
#ifndef _MSC_VER
    for(boost::filesystem::directory_iterator itr("/"); itr != end_itr; ++itr)
    {
        if(data == itr->path().string())
        {
            for (boost::filesystem::directory_iterator itr2(itr->path()); itr2 != end_itr; ++itr2)
            {
                _sp->addSuggestion(itr2->path().string());
            }
            _current_path = data + "/";
            return;
        }
    }
#endif
}

FileBrowseWidget::FileBrowseWidget(WContainerWidget* parent,
                 const std::string& upload_dir,
                 const boost::filesystem::path & current_dir):
    AutoCompleteFileWidget(parent, upload_dir, current_dir)
{
    _btn_browse = new WFileUpload(this);
    _btn_browse->setProgressBar(new WProgressBar());
    _btn_browse->setFileTextSize(50*1024*1024);
    _btn_browse->uploaded().connect(std::bind([this]()
    {
        const std::vector<Http::UploadedFile>& files =
                _btn_browse->uploadedFiles();
        if(files.size() == 1)
        {
            std::string spooled_name = files[0].spoolFileName();
            //std::string host_name = files[0].clientFileName();
            _txt_manual_entry->setText(spooled_name);
            _sig_file_selected.emit();
        }
    }));
    _btn_browse->changed().connect(std::bind([this]()
    {
        _btn_browse->upload();
    }));
}
std::string AutoCompleteFileWidget::hostFile()
{
    return _txt_manual_entry->text().toUTF8();
}

std::string FileBrowseWidget::clientFile()
{
    return _btn_browse->clientFileName().toUTF8();
}
