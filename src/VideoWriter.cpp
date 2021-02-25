#include "VideoWriter.hpp"
#include "Types.hpp"


void Video_Writer::CheckIsOpen()
{
    if (!m_cvWriter.isOpened())
    {
        exit(EXIT_FAILURE);
    }
}

void Video_Writer::Init(const std::string& outputVideo, double fps, int width, int height)
{
    m_ready = m_cvWriter.open(outputVideo,
                              cv::VideoWriter::fourcc('M', 'P', 'E', 'G'),
                              fps,
                              cv::Size(width, height),
                              true);
    CheckIsOpen();
}

void Video_Writer::WriteFrame(std::shared_ptr<cv::Mat>& frame)
{
    if(m_cvWriter.isOpened())
    {
        /*cv::cvtColor(*frame, *frame, cv::COLOR_RGB2BGR);*/
        m_cvWriter.write(*frame);
    }
}

bool Video_Writer::IsReady() const
{
    return m_ready;
}

void Video_Writer::Close()
{
    m_cvWriter.release();
}

