#include "VideoProcessor.h"
#include <opencv2/videoio.hpp>
VideoProcessor::VideoProcessor(QObject *parent /*= 0*/)
	:QObject(parent)
	,delay(-1)
	,rate(0)
	,fnumber(0)
	,length(0)
	,stop(true)
	,modify(false)
	,curPos(0)
	,curIndex(0)
	,digits(0)
	,extension(".avi")
	,levels(4)
	,alpha(10)
	,lambda_c(80),
	fl(0.05),
	fh(0.4),
	chromattenuation(0.1),
	delta(0),
	exaggeration_factor(2.0),
	lambda(0)
{
	connect(this, &VideoProcessor::revert, this, &VideoProcessor::revertVideo);
}

bool VideoProcessor::isStop()
{
	return stop;
}

bool VideoProcessor::isModified()
{
	return modify;
}

bool VideoProcessor::isOpen()
{
	return capture.isOpened();
}

bool VideoProcessor::setDelay(int d)
{
	delay = d;
	return false;
}

long VideoProcessor::getNumberOfProcessedFrames()
{
	return fnumber;
}

long VideoProcessor::getNumberOfPlayedFrames()
{
	return curPos;

}

cv::VideoCapture VideoProcessor::getCaptrue()
{
	return VideoCapture();
}

cv::Size VideoProcessor::getFrameSize()
{
	int w = capture.get(CAP_PROP_FRAME_WIDTH);
	int h = capture.get(CAP_PROP_FRAME_HEIGHT);
	return cv::Size(w,h);
}

long VideoProcessor::getFrameNumber()
{
	return static_cast<long>(capture.get(CAP_PROP_POS_FRAMES));
}

double VideoProcessor::getPositionMS()
{
	return capture.get(CAP_PROP_POS_MSEC);
}

double VideoProcessor::getFrameRate()
{
	return capture.get(CAP_PROP_FPS);
}

long VideoProcessor::getLength()
{
	return length;
}

double VideoProcessor::getLengthMS()
{
	return 1000.0*length / rate;
}

int VideoProcessor::getCodec(char codec[4])
{
	union MyUnion
	{
		int value;
		char code[4];
	}returned;
	returned.value = static_cast<int>(capture.get(CAP_PROP_FOURCC));
	codec[0] = returned.code[0];
	codec[1] = returned.code[1];
	codec[2] = returned.code[2];
	codec[3] = returned.code[3];
	return returned.value;
}

void VideoProcessor::getTempFile(std::string &)
{

}

void VideoProcessor::getCurTempFile(std::string &)
{

}

void VideoProcessor::setRelativeRosition(double pos)
{

}

bool VideoProcessor::setInput(std::string &filename)
{
	fnumber = 0;
	tempFile = filename;
	if (isOpen())
	{
		capture.release();
	}
	if (capture.open(filename))
	{
		length = capture.get(CAP_PROP_FRAME_COUNT);
		rate = getFrameRate();
		Mat input;
		getNexFrame(input);
		emit showFrame(input);
		emit updateBtn();
		return true;
	}
	return false;
}

bool VideoProcessor::setOutput(const std::string& filename, int codec /*= 0*/, double framerate /*= 0.0*/, bool isColor /*= true*/)
{
	outputFile = filename;
	extension.clear();

	if (framerate==0.0)
	{
		framerate = getFrameRate();
	}
	char c[4];
	if (codec==0)
	{
		getCodec(c);
	}
	return writer.open(outputFile,codec,framerate,getFrameSize(),isColor);
}

bool VideoProcessor::setOutput(const std::string& filename, const std::string&ext, int numberofDigits /*= 3*/, int startIndex /*= 0*/)
{
	if (numberofDigits<0)
	{
		return false;
	}
	outputFile = filename;
	extension = ext;
	digits = numberofDigits;
	curIndex = startIndex;
	return true;
}

void VideoProcessor::setSpatialFilter(spatialFilterType type)
{

}

void VideoProcessor::setTemporalFilter(temporalFilterType type)
{

}

void VideoProcessor::playIt()
{
	Mat input;
	if (!isOpen())
	{
		return;
	}
	stop = false;
	emit updateBtn();
	while (!isStop())
	{
		if (!getNexFrame(input))
		{
			break;
		}
		curPos = capture.get(CAP_PROP_POS_FRAMES);
		emit showFrame(input);
		emit updateProgressBar();
		emit sleep(delay);
	}
	if (!isStop())
	{
		emit revert();
	}
}

void VideoProcessor::pauseIt()
{
	stop = true;
	emit updateBtn();
}

void VideoProcessor::stopIt()
{
	stop = true;
	emit revert();
}

void VideoProcessor::prevFrame()
{
	if (isStop())
	{
		pauseIt();
	}
	if (curPos>=0)
	{
		curPos -= 100;
		jumpTo(curPos);
	}
	emit updateProgressBar();
}

void VideoProcessor::nextFrame()
{
	if (isStop())
	{
		pauseIt();
	}
	curPos += 100;
	if (curPos<=length)
	{
		curPos += 100;
		jumpTo(curPos);
	}
	emit updateProgressBar();
}

bool VideoProcessor::jumpTo(long index)
{
	if (index>=length)
	{
		return true;
	}
	Mat frame;
	bool re = capture.set(CAP_PROP_POS_FRAMES, index);
	if (re&&!isStop())
	{
		capture.read(frame);
		emit showFrame(frame);
	}
	return re;
}

bool VideoProcessor::jumpToMS(double pos)
{
	return capture.set(CAP_PROP_POS_MSEC,pos);
}

void VideoProcessor::close()
{
	rate = 0;
	length = 0;
	modify = 0;
	capture.release();
	writer.release();
	tempWriter.release();
}

void VideoProcessor::motionManify()
{

}

void VideoProcessor::colorManify()
{

}

void VideoProcessor::writeOutput()
{

}

void VideoProcessor::revertVideo()
{
	jumpTo(0);
	curPos = 0;
	pauseIt();
	emit updateProgressBar();
}




void VideoProcessor::calculateLenght()
{

}

bool VideoProcessor::getNexFrame(Mat &frame)
{
	return capture.read(frame);
}

bool VideoProcessor::writeNextFrame(Mat &frame)
{
	return false;
}

bool VideoProcessor::createTemp(double framerate /*= 0.0*/, bool isColor /*= true*/)
{
	return false;
}

bool VideoProcessor::spatialFilter(Mat &src, std::vector<Mat> &pyramid)
{
	return false;
}

void VideoProcessor::temporalFilter(Mat& src, Mat& dst)
{

}

void VideoProcessor::temporalIIRFilter(Mat& src, Mat& dst)
{

}

void VideoProcessor::temporalIdealFilter(Mat& src, Mat& dst)
{

}

void VideoProcessor::aplify(Mat& src, Mat& dst)
{

}

void VideoProcessor::attenuate(Mat&src, Mat&dst)
{

}

void VideoProcessor::concat(const std::vector<Mat> &frames, Mat dst)
{

}

void VideoProcessor::deConcat(const Mat& src, const cv::Size &frameSize, std::vector<Mat>& frames)
{

}

void VideoProcessor::createIdealBandPassFilter(Mat &filter, double fl, double fh, double rate)
{

}
