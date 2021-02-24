#include "VideoReader.hpp"

std::shared_ptr<cv::Mat> VideoReader::ReadFrame()
{
    // opencv copies data anyway
    cv::Mat captureFrame;
    m_capture.read(captureFrame);
    return std::make_shared<cv::Mat>(std::move(captureFrame));
}

bool VideoReader::IsExhausted(const std::shared_ptr<cv::Mat>& frame) const
{
    assert(frame!=nullptr);
    return frame->empty();
}

void VideoReader::CheckIsOpen(const std::string& source)
{
    if (!m_capture.isOpened())
    {
        throw std::runtime_error("Failed to open video capture for the source = " + source);
    }
}

void VideoReader::Init(const std::string& source)
{
    m_capture.open(source);
    CheckIsOpen(source);
}

int VideoReader::GetSourceWidth() const
{
    return m_capture.get(cv::CAP_PROP_FRAME_WIDTH);
}

int VideoReader::GetSourceHeight() const
{
    return m_capture.get(cv::CAP_PROP_FRAME_HEIGHT);
}

double VideoReader::GetSourceFps() const
{
    return m_capture.get(cv::CAP_PROP_FPS);
}

bool VideoReader::ConvertToRGB()
{
    m_capture.set(cv::CAP_PROP_CONVERT_RGB, 1.0);
    return static_cast<bool>(m_capture.get(cv::CAP_PROP_CONVERT_RGB));
}

std::string VideoReader::GetSourceEncoding() const
{
    char fourccStr[5];
    auto fourcc = (int)m_capture.get(cv::CAP_PROP_FOURCC);
    sprintf(fourccStr,"%c%c%c%c",fourcc & 0xFF, (fourcc >> 8) & 0xFF, (fourcc >> 16) & 0xFF, (fourcc >> 24) & 0xFF);
    return fourccStr;
}

int VideoReader::GetSourceEncodingInt() const
{
    return (int)m_capture.get(cv::CAP_PROP_FOURCC);
}

int VideoReader::GetFrameCount() const
{
    return static_cast<int>(lround(m_capture.get(cv::CAP_PROP_FRAME_COUNT)));
};

void VideoReader::Close()
{
    m_capture.release();
}

std::shared_ptr<cv::Mat> VideoReaderRgbWrapper::ReadFrame()
{
    auto framePtr = m_reader->ReadFrame();
    if (!IsExhausted(framePtr))
    {
        cv::cvtColor(*framePtr, *framePtr, cv::COLOR_BGR2RGB);
    }
    return framePtr;
}

bool VideoReaderRgbWrapper::IsExhausted(const std::shared_ptr<cv::Mat>& frame) const
{
    return m_reader->IsExhausted(frame);
}

VideoReaderRgbWrapper::VideoReaderRgbWrapper(std::unique_ptr<VideoReader> reader):
        m_reader(std::move(reader))
{}
